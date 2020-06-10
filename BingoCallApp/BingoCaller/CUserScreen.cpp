// CUserScreen.cpp : implementation file
//

#include "pch.h"
#include "BingoCaller.h"
#include "CUserScreen.h"
#include "afxdialogex.h"
#include "BingoCallerDlg.h"

int NUMBER_PANEL_USER_ITEM_WIDTH = 50;
int NUMBER_PANEL_USER_ITEM_HEIGHT = 60;
int NUMBER_PANEL_USER_ITEM_INTERVAL = 10;

#define NUMBER_PANEL_COL_COUNT 15
#define MARGIN_LEFT 100
#define MARGIN_RIGHT 100
#define MARGIN_TOP 30
#define MARGIN_BOTTOM 100

IMPLEMENT_DYNAMIC(CUserScreen, CDialogEx)

struct st_number_user_panel
{
	CRect rect;
	int number;
	int bmatched;
	st_number_user_panel() {
		bmatched = 0;
	}
};

st_number_user_panel g_number_user_panel[90 / NUMBER_PANEL_COL_COUNT][NUMBER_PANEL_COL_COUNT];

CUserScreen::CUserScreen(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_USER_SCREEN, pParent)
{
	m_pParentWnd = (CDialog*)pParent;
	m_nCalledLastNumber = 0;
	m_nNumbersCalled = 0;
	m_nGameNumber = 1;
	m_nGameLevel = 1;
	m_szAdText[0] = 0;
	m_bVerified = false;
	m_szVerifiedString[0] = 0;
	m_nverifiedPanelId = 0;
}

CUserScreen::~CUserScreen()
{
}

void CUserScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserScreen, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()

	ON_BN_CLICKED(IDOK, &CUserScreen::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserScreen message handlers


void CUserScreen::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	CDialogEx::OnOK();
}


BOOL CUserScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowWindow(SW_MAXIMIZE);
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);
	m_nNumPanelRect.left = MARGIN_LEFT;
	m_nNumPanelRect.top = MARGIN_TOP;
	m_nNumPanelRect.right = rect.right - MARGIN_RIGHT;
	m_nNumPanelRect.bottom = rect.bottom * 0.6;

	NUMBER_PANEL_USER_ITEM_WIDTH = (m_nNumPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL * 1.5 * 2) / NUMBER_PANEL_COL_COUNT; // - NUMBER_PANEL_USER_ITEM_INTERVAL * 2 - NUMBER_PANEL_USER_ITEM_INTERVAL * (NUMBER_PANEL_COL_COUNT - 1)
	NUMBER_PANEL_USER_ITEM_HEIGHT = (m_nNumPanelRect.Height() - NUMBER_PANEL_USER_ITEM_INTERVAL * 2) / (90 / NUMBER_PANEL_COL_COUNT); // - NUMBER_PANEL_USER_ITEM_INTERVAL * 2 - NUMBER_PANEL_USER_ITEM_INTERVAL * (90 / NUMBER_PANEL_COL_COUNT - 1)
	makeNumberPanel();
	setPanelRect();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserScreen::OnSize(UINT nType, int cx, int cy)
{
	ShowWindow(SW_MAXIMIZE);
}

void CUserScreen::OnPaint()
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


void CUserScreen::OnDraw(CDC* dc)
{
	drawYellowBoxes(dc);
	CRect rect;
	HPEN pen;
	LOGFONT lf; CFont font, font1, font2, font3;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 270; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));
	font.CreateFontIndirect(&lf);
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 140; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));
	font2.CreateFontIndirect(&lf);
	CFont* oldfont = dc->SelectObject(&font2);
	lf.lfHeight = 280;
	font3.CreateFontIndirect(&lf);

	pen = CreatePen(PS_SOLID, NUMBER_PANEL_USER_ITEM_INTERVAL/2, RGB(50, 50, 50));
	SelectObject(dc->m_hDC, pen);

	COLORREF clr = RGB(0, 103, 177);
	CBrush br1, br2, br3, * br;
	br1.CreateSolidBrush(clr);
	br2.CreateSolidBrush(RGB(234, 34, 24));
	br3.CreateSolidBrush(RGB(61, 226, 159));
	CBrush* oldBrush = dc->SelectObject(&br1);
	TCHAR str_num[3];
	SetBkMode(dc->m_hDC, TRANSPARENT);

	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 80; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));
	font1.CreateFontIndirect(&lf);
	dc->SelectObject(&font1);
	dc->SetTextColor(RGB(255, 255, 255));
	for (int i = 0; i < 90 / NUMBER_PANEL_COL_COUNT; i++)
	{
		for (int j = 0; j < NUMBER_PANEL_COL_COUNT; j++)
		{
			if (g_number_user_panel[i][j].bmatched==2)
				br = &br3;
			else if (g_number_user_panel[i][j].bmatched==1)
				br = &br2;
			else
				br = &br1;
			dc->SelectObject(br);
			dc->Rectangle(g_number_user_panel[i][j].rect);
			_stprintf_s(str_num, _T("%d"), g_number_user_panel[i][j].number);
			DrawText(dc->m_hDC, str_num, -1, g_number_user_panel[i][j].rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	dc->SelectObject(&oldfont);
	dc->SelectObject(oldBrush);
	font.DeleteObject();
	DeleteObject(pen);

}


void CUserScreen::makeNumberPanel()
{
	for (int i = 0; i < 90 / NUMBER_PANEL_COL_COUNT; i++)
	{
		for (int j = 0; j < NUMBER_PANEL_COL_COUNT; j++)
		{
			g_number_user_panel[i][j].number = i * NUMBER_PANEL_COL_COUNT + j + 1;
			g_number_user_panel[i][j].rect.left = m_nNumPanelRect.left + 1.5 * NUMBER_PANEL_USER_ITEM_INTERVAL + j * (NUMBER_PANEL_USER_ITEM_WIDTH);
			g_number_user_panel[i][j].rect.right = g_number_user_panel[i][j].rect.left + NUMBER_PANEL_USER_ITEM_WIDTH;
			g_number_user_panel[i][j].rect.top = m_nNumPanelRect.top + 1 * NUMBER_PANEL_USER_ITEM_INTERVAL + i * NUMBER_PANEL_USER_ITEM_HEIGHT;
			g_number_user_panel[i][j].rect.bottom = g_number_user_panel[i][j].rect.top + NUMBER_PANEL_USER_ITEM_HEIGHT;
			g_number_user_panel[i][j].bmatched = 0;
		}
	}
}


void CUserScreen::resetNumberArray()
{
	for (int i = 0; i < 90 / NUMBER_PANEL_COL_COUNT; i++)
	{
		for (int j = 0; j < NUMBER_PANEL_COL_COUNT; j++)
		{
			g_number_user_panel[i][j].bmatched = 0;
		}
	}
}


void CUserScreen::drawYellowBoxes(CDC *dc)
{
	CRect rect;
	GetClientRect(rect);
	COLORREF clr = RGB(255, 196, 37);
	CBrush br1, br2, br3, br4, br5, * br;
	br1.CreateSolidBrush(clr);
	br2.CreateSolidBrush(RGB(61, 226, 159));
	br3.CreateSolidBrush(RGB(255, 255, 255));
	br4.CreateSolidBrush(RGB(0, 0, 0));
	br5.CreateSolidBrush(RGB(255, 215, 0));
	CBrush* oldBrush = dc->SelectObject(&br1);
	dc->Rectangle(m_nNumPanelRect);
	dc->SetBkMode(TRANSPARENT);
	LOGFONT lf; CFont font, font1, font2, font3;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 250; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));
	font.CreateFontIndirect(&lf);
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 60; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));
	font1.CreateFontIndirect(&lf);
	CFont* oldfont = dc->SelectObject(&font1);
	lf.lfHeight = 40;
	font3.CreateFontIndirect(&lf);
	dc->Rectangle(m_LastNumberRect);
	CRect rect1;
	rect1.left = m_LastNumberRect.left + 20;
	rect1.right = m_LastNumberRect.right - 20;
	rect1.top = m_LastNumberRect.top + m_LastNumberRect.Height() * 0.8;
	rect1.bottom = m_LastNumberRect.bottom - 5;
	dc->SelectObject(&br3);
	dc->Rectangle(rect1);
	dc->SetTextColor(RGB(0, 0, 0));
	DrawText(dc->m_hDC, _T("LAST NUMBER"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	TCHAR str[50];
	if (m_nCalledLastNumber > 0)
		_stprintf_s(str, _T("%d"), m_nCalledLastNumber);
	else
		_stprintf_s(str, _T(""));
	rect1.right = m_LastNumberRect.right-16;
	rect1.top = m_LastNumberRect.top;
	rect1.bottom = m_LastNumberRect.top + m_LastNumberRect.Height() * 0.8-16;
	dc->SelectObject(&font);
	dc->SetTextColor(RGB(0, 0, 0));
	DrawText(dc->m_hDC, str, -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	rect1.right = m_LastNumberRect.right - 20;
	rect1.top = m_LastNumberRect.top;
	rect1.bottom = m_LastNumberRect.top + m_LastNumberRect.Height() * 0.8-20;
	dc->SetTextColor(RGB(234, 34, 24));
	DrawText(dc->m_hDC, str, -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	dc->SelectObject(&br3);
	dc->Rectangle(m_LogoPanelRect);

	rect1.left = m_LogoPanelRect.left + NUMBER_PANEL_USER_ITEM_INTERVAL;
	rect1.right = rect1.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) / 3 - NUMBER_PANEL_USER_ITEM_INTERVAL;
	rect1.top = m_LogoPanelRect.top + m_LogoPanelRect.Height() * 0.8;
	rect1.bottom = m_LogoPanelRect.bottom - 5;
	dc->SelectObject(&br1);
	dc->Rectangle(rect1);
	dc->SelectObject(&br3);
	dc->Rectangle(m_GameNumberRect);
	dc->SelectObject(&font3);
	dc->SetTextColor(RGB(0, 0, 0));
	rect1.right = m_GameNumberRect.left;
	DrawText(dc->m_hDC, _T("Game Number:"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	_stprintf_s(str, _T("%d"), m_nGameNumber);
	DrawText(dc->m_hDC, str, -1, m_GameNumberRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	rect1.left = m_LogoPanelRect.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) / 3 + NUMBER_PANEL_USER_ITEM_INTERVAL;
	rect1.right = rect1.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) / 3 - NUMBER_PANEL_USER_ITEM_INTERVAL;
	rect1.top = m_LogoPanelRect.top + m_LogoPanelRect.Height() * 0.8;
	rect1.bottom = m_LogoPanelRect.bottom - 5;
	dc->SelectObject(&br1);
	dc->Rectangle(rect1);
	CBrush br_color_pad;
	br_color_pad.CreateSolidBrush(m_color);
	dc->SelectObject(&br_color_pad);
	dc->Rectangle(m_colorRect);
	dc->SelectObject(&font3);
	dc->SetTextColor(RGB(0, 0, 0));
	rect1.right = m_colorRect.left;
	DrawText(dc->m_hDC, _T("Ticket Colour:"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	if (_tcscmp(m_szColor, _T("blue")) == 0 || _tcscmp(m_szColor, _T("brown")) == 0 || _tcscmp(m_szColor, _T("purple")) == 0)
		dc->SetTextColor(RGB(255, 255, 255));
	else
		dc->SetTextColor(RGB(0, 0, 0));
	_tcscpy_s(str, m_szColor);
	DrawText(dc->m_hDC, str, -1, m_colorRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	rect1.left = m_LogoPanelRect.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL)*2 / 3 + NUMBER_PANEL_USER_ITEM_INTERVAL;
	rect1.right = rect1.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) / 3 - NUMBER_PANEL_USER_ITEM_INTERVAL;
	rect1.top = m_LogoPanelRect.top + m_LogoPanelRect.Height() * 0.8;
	rect1.bottom = m_LogoPanelRect.bottom - 5;
	dc->SelectObject(&br1);
	dc->Rectangle(rect1);
	dc->SelectObject(&br3);
	dc->Rectangle(m_GameLevelRect);
	dc->SelectObject(&font3);
	dc->SetTextColor(RGB(0, 0, 0));
	rect1.right = m_GameLevelRect.left;
	DrawText(dc->m_hDC, _T("Playing For:"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	if (m_nGameLevel == 1)
		_tcscpy_s(str, _T("One Line"));
	if (m_nGameLevel == 2)
		_tcscpy_s(str, _T("Two Line"));
	if (m_nGameLevel == 3)
		_tcscpy_s(str, _T("Full House"));
	DrawText(dc->m_hDC, str, -1, m_GameLevelRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	rect1.left = m_advertiseRect.right + NUMBER_PANEL_USER_ITEM_INTERVAL;
	rect1.right = rect1.left+ m_nNumPanelRect.Width() * 1.2 / NUMBER_PANEL_COL_COUNT;
	rect1.top = m_advertiseRect.top;
	rect1.bottom = m_advertiseRect.bottom;
	dc->SelectObject(&br3);
	dc->Rectangle(rect1);
	DrawText(dc->m_hDC, _T("Numbers Called"), -1, rect1, DT_CENTER | DT_WORDBREAK| DT_CALCRECT);
	rect1.left += 10;
	rect1.right += 10;
	DrawText(dc->m_hDC, _T("Numbers Called"), -1, rect1, DT_CENTER | DT_WORDBREAK);
	dc->SelectObject(&br1);
	dc->Rectangle(m_NumbersCalledRect);
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 100; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));
	font2.CreateFontIndirect(&lf);
	dc->SelectObject(&font2);
	_stprintf_s(str, _T("%d"), m_nNumbersCalled);
	dc->SetTextColor(RGB(255, 255, 255));
	DrawText(dc->m_hDC, str, -1, m_NumbersCalledRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (PathFileExists(m_szLogoFile) && !m_bVerified)
	{
		m_imgLogo.Load(m_szLogoFile);
		m_imgLogo.StretchBlt(dc->m_hDC, m_LogoPanelRect, SRCCOPY);
		m_imgLogo.Detach();
	}
	else if (m_bVerified)
	{
/*
		dc->SelectObject(&font1);
		dc->SetTextColor(RGB(0, 0, 0));
		rect1 = m_LogoPanelRect;
		rect1.left = m_LogoPanelRect.left + 50;
		DrawText(dc->m_hDC, m_szVerifiedString, -1, m_LogoPanelRect, DT_LEFT|DT_WORDBREAK);
*/
		int item_height = 50;
		int item_width = 50;
		int margin_left = 50;
		int margin_top = 30;
		HPEN pen = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
		CBrush br01, br02, br03, * br;
		br01.CreateSolidBrush(RGB(0, 103, 177));
		br02.CreateSolidBrush(RGB(234, 34, 24));
		br03.CreateSolidBrush(RGB(61, 226, 159));
		SelectObject(dc->m_hDC, pen);
		TCHAR str_num[50];
		for (int i = 0; i < 9; i++)
		{
			for (int j=0;j<3;j++)
			{
				rect1.left = m_LogoPanelRect.left + margin_left + item_width * i;
				rect1.right = rect1.left + item_width;
				rect1.top = m_LogoPanelRect.top + margin_top + item_height * j;
				rect1.bottom = rect1.top + item_height;
				dc->SelectObject(&br01);
				if (((CBingoCallerDlg*)m_pParentWnd)->checkInCalledNumbers(m_verifiedPanel[i][j]))
				{
					dc->SelectObject(&br02);
					if (m_verifiedPanel[i][j] == m_nCalledLastNumber)
						dc->SelectObject(&br03);
				}
				dc->Rectangle(rect1);
				dc->SelectObject(&font3);
				dc->SetTextColor(RGB(255, 255, 255));
				if (m_verifiedPanel[i][j] != 0)
					_stprintf_s(str_num, _T("%d"), m_verifiedPanel[i][j]);
				else
					str_num[0] = 0;
				DrawText(dc->m_hDC, str_num, -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

			}
		}
		rect1.left = m_LogoPanelRect.left + margin_left + item_width * 9 + margin_left;
		rect1.right = rect1.left + 200;
		rect1.top = m_LogoPanelRect.top +margin_top;
		rect1.bottom = rect1.top + item_height;
		_stprintf_s(str_num, _T("panel id : %d"), m_nverifiedPanelId);
		dc->SetTextColor(RGB(0, 0, 0));
		DrawText(dc->m_hDC, str_num, -1, rect1, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
		DeleteObject(pen);
	}

	if (PathFileExists(m_szAdFile))
	{
		m_imgAd.Load(m_szAdFile);
		m_imgAd.StretchBlt(dc->m_hDC, m_advertiseRect, SRCCOPY);
		m_imgAd.Detach();
	}
	else
	{
		dc->SelectObject(&br3);
		dc->Rectangle(m_advertiseRect);
	}
	dc->SetTextColor(RGB(0, 0, 0));
	rect1 = m_advertiseRect;
	rect1.top -= 10;
	rect1.bottom -= 20;
	DrawText(dc->m_hDC, m_szAdText, -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	
	dc->SelectObject(&oldfont);
	dc->SelectObject(&oldBrush);
}


void CUserScreen::setPanelMatchInfo(int number, int bmatch)
{
	g_number_user_panel[(number - 1) / NUMBER_PANEL_COL_COUNT][(number - 1) % NUMBER_PANEL_COL_COUNT].bmatched = bmatch;
	InvalidateRect(g_number_user_panel[(number - 1) / NUMBER_PANEL_COL_COUNT][(number - 1) % NUMBER_PANEL_COL_COUNT].rect);
}


void CUserScreen::resetPanel()
{
	resetNumberArray();
	Invalidate(FALSE);
}


void CUserScreen::setPanelRect()
{
	CRect rect;
	GetClientRect(rect);
	m_LastNumberRect.left = m_nNumPanelRect.left;
	m_LastNumberRect.right = m_nNumPanelRect.left + m_nNumPanelRect.Width() / 4;
	m_LastNumberRect.top = m_nNumPanelRect.bottom + NUMBER_PANEL_USER_ITEM_INTERVAL * 1.5;
	m_LastNumberRect.bottom = m_LastNumberRect.top + rect.Height() * 0.25;

	m_LogoPanelRect.left = m_LastNumberRect.right + NUMBER_PANEL_USER_ITEM_INTERVAL * 2;
	m_LogoPanelRect.right = m_nNumPanelRect.right;
	m_LogoPanelRect.top = m_LastNumberRect.top;
	m_LogoPanelRect.bottom = m_LastNumberRect.bottom;

	int m_logoItemTop = m_LogoPanelRect.top + m_LogoPanelRect.Height() * 0.8;
	m_GameNumberRect.left = m_LogoPanelRect.left + NUMBER_PANEL_USER_ITEM_INTERVAL + 0 * m_LogoPanelRect.Width() / 3 + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) *2 / 9;
	m_GameNumberRect.right = m_GameNumberRect.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) / 9 - NUMBER_PANEL_USER_ITEM_INTERVAL*2;
	m_GameNumberRect.top = m_logoItemTop + 5;
	m_GameNumberRect.bottom = m_LogoPanelRect.bottom - 5 - 5;

	m_colorRect.left = m_LogoPanelRect.left + NUMBER_PANEL_USER_ITEM_INTERVAL + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) / 3 + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) * 2 / 9;
	m_colorRect.right = m_colorRect.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) / 9 - NUMBER_PANEL_USER_ITEM_INTERVAL * 2;
	m_colorRect.top = m_logoItemTop + 5;
	m_colorRect.bottom = m_LogoPanelRect.bottom - 5 - 5;

	m_GameLevelRect.left = m_LogoPanelRect.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) * 2 / 3 + NUMBER_PANEL_USER_ITEM_INTERVAL + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL) * 1.5 / 9;
	m_GameLevelRect.right = m_GameLevelRect.left + (m_LogoPanelRect.Width() - NUMBER_PANEL_USER_ITEM_INTERVAL)*1.5 / 9 - NUMBER_PANEL_USER_ITEM_INTERVAL * 2;
	m_GameLevelRect.top = m_logoItemTop + 5;
	m_GameLevelRect.bottom = m_LogoPanelRect.bottom - 5 - 5;

	m_advertiseRect.left = m_nNumPanelRect.left;
	m_advertiseRect.right = m_advertiseRect.left + m_nNumPanelRect.Width() * 4 / 5 - NUMBER_PANEL_USER_ITEM_INTERVAL*2;
	m_advertiseRect.top = m_LastNumberRect.bottom + NUMBER_PANEL_USER_ITEM_INTERVAL * 1.5;
	m_advertiseRect.bottom = rect.bottom - 30;

	m_NumbersCalledRect.left = m_advertiseRect.right + NUMBER_PANEL_USER_ITEM_INTERVAL * 2 + m_nNumPanelRect.Width()*1.2/ NUMBER_PANEL_COL_COUNT;
	m_NumbersCalledRect.right = m_nNumPanelRect.right;
	m_NumbersCalledRect.top = m_advertiseRect.top;
	m_NumbersCalledRect.bottom = m_advertiseRect.bottom;
}


void CUserScreen::addToVerified(int panel_id, int level)
{
	if (level == 1)
	{
		bool bexist = false;
		for (int i = 0; i < m_arrOneLineVerified.GetSize(); i++)
		{
			if (m_arrOneLineVerified.GetAt(i) == panel_id)
			{
				bexist = true;
				break;
			}
		}
		if(!bexist)
			m_arrOneLineVerified.Add(panel_id);
	}
	else if (level == 2)
	{
		bool bexist = false;
		for (int i = 0; i < m_arrTwoLineVerified.GetSize(); i++)
		{
			if (m_arrTwoLineVerified.GetAt(i) == panel_id)
			{
				bexist = true;
				break;
			}
		}
		if (!bexist)
			m_arrTwoLineVerified.Add(panel_id);
	}
	else if (level == 3)
	{
		bool bexist = false;
		for (int i = 0; i < m_arrFullHouseVerified.GetSize(); i++)
		{
			if (m_arrFullHouseVerified.GetAt(i) == panel_id)
			{
				bexist = true;
				break;
			}
		}
		if (!bexist)
			m_arrFullHouseVerified.Add(panel_id);
	}
	TCHAR szOneline[1000]; szOneline[0] = 0;
	for (int i = 0; i < m_arrOneLineVerified.GetSize(); i++)
	{
		if (i == 0)
			_tcscpy_s(szOneline, _T("One Line matched:"));
		int number = m_arrOneLineVerified.GetAt(i);
		if (i == 0)
			_stprintf_s(szOneline, _T("%s %d"), szOneline, number);
		else
			_stprintf_s(szOneline, _T("%s, %d"), szOneline, number);
	}
	TCHAR szTwoline[1000]; szTwoline[0] = 0;
	for (int i = 0; i < m_arrTwoLineVerified.GetSize(); i++)
	{
		if (i == 0)
			_tcscpy_s(szTwoline, _T("Two Line matched:"));
		int number = m_arrTwoLineVerified.GetAt(i);
		if (i == 0)
			_stprintf_s(szTwoline, _T("%s %d"), szTwoline, number);
		else
			_stprintf_s(szTwoline, _T("%s, %d"), szTwoline, number);
	}
	TCHAR szFullHouse[1000]; szFullHouse[0] = 0;
	for (int i = 0; i < m_arrFullHouseVerified.GetSize(); i++)
	{
		if (i == 0)
			_tcscpy_s(szFullHouse, _T("Full House matched:"));
		int number = m_arrFullHouseVerified.GetAt(i);
		if (i == 0)
			_stprintf_s(szFullHouse, _T("%s %d"), szFullHouse, number);
		else
			_stprintf_s(szFullHouse, _T("%s, %d"), szFullHouse, number);
	}
	for (int i = 0; i < 3; i++)
	{
		if (i == 0 && _tcscmp(szOneline, _T("")))
		{
			_stprintf_s(m_szVerifiedString, _T("%s\r\n"), szOneline);
		}
		else if(i==0)
			continue;
		if (i == 1 && _tcscmp(szTwoline, _T("")))
			_stprintf_s(m_szVerifiedString, _T("%s%s\r\n"), m_szVerifiedString, szTwoline);
		else if(i==1)
			continue;
		if (i == 2 && _tcscmp(szFullHouse, _T("")))
			_stprintf_s(m_szVerifiedString, _T("%s%s"), m_szVerifiedString, szFullHouse);
	}
	InvalidateRect(m_LogoPanelRect);
}


void CUserScreen::resetVerifiedArray()
{
	m_arrOneLineVerified.RemoveAll();
	m_arrTwoLineVerified.RemoveAll();
	m_arrFullHouseVerified.RemoveAll();
	m_szVerifiedString[0] = 0;
}
