
// BingoCallerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BingoCaller.h"
#include "BingoCallerDlg.h"
#include "afxdialogex.h"
#include "CPanelViewDlg.h"
#include "CSelectColorDlg.h"
#include <mysql.h>

#define SERVER_PORT 1001
#define BUFFER_SIZE 4096

 #define TICKET_ONLEY


#define IPAddress "127.0.0.1"
SOCKET g_ServerSocket;
SOCKET g_ClientSocket;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BALL_BUTTON_CUT_MARGIN 2

#define WM_START WM_USER + 1
#define WM_STOP WM_USER + 2
#define WM_CALL WM_USER + 3
#define WM_VERIFY WM_USER + 4
#define WM_RECALL WM_USER + 5

#define ONE_LINE 1
#define TWO_LINE 2
#define FULL_HOUSE 3

#define NUMBER_PANEL_COL_COUNT 15

#define MAX_RANGE_COUNT 2700000

MYSQL* mysql;
MYSQL_RES* res;
MYSQL_ROW row;

int g_nColumnNumberCount[9] = { 9, 10, 10, 10, 10, 10, 10, 10, 11 };
int g_nNumberPanelMargin = 50;
int g_nNumberPanelTopMargin = 40;

int NUMBER_PANEL_ITEM_WIDTH = 42;
int NUMBER_PANEL_ITEM_HEIGHT = 65;
int NUMBER_PANEL_ITEM_INTERVAL = 5;

struct st_number_user_panel;
extern st_number_user_panel g_number_user_panel[90 / NUMBER_PANEL_COL_COUNT][NUMBER_PANEL_COL_COUNT];

bool checkInNumberArr(int* arr, int cnt, int val);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBingoCallerDlg dialog

struct st_number_panel
{
	CRect rect;
	int number;
	bool bCalled;
	st_number_panel() {
		bCalled = false;
	}
};

st_number_panel g_number_panel[90/ NUMBER_PANEL_COL_COUNT][NUMBER_PANEL_COL_COUNT];

struct st_panel_item_match_info
{
	BYTE val;
	bool bmatch;
};

struct st_panel_info
{
	int panel_id;
	st_panel_item_match_info items[9][3];
};

struct st_color_info
{
	TCHAR color_name[20];
	COLORREF color;
};

st_color_info color_info[30];
st_panel_info g_panel_info[MAX_RANGE_COUNT];
int g_nPanelInfoCnt = 0;
int g_npMatchedPanels[1000];
int g_nMatchedPanelCnt = 0;

CArray<st_recall_info> g_recall_info;
int g_nRecallCount = 0;
CBingoCallerDlg* g_pCallerDlg = 0;

CBingoCallerDlg::CBingoCallerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BINGOCALLER_DIALOG, pParent)
	, m_nPanelNumber(0)
	, m_szOneLine(_T(""))
	, m_szTwoLine(_T(""))
	, m_szFullHouse(_T(""))
	, m_nPanelRangeFrom(1)
	, m_nPanelRangeTo(100)
	, m_szOneLineMiss(_T(""))
	, m_szTwoLineMiss(_T(""))
	, m_szFullHouseMiss(_T(""))
	, m_bOneLineWinner(TRUE)
	, m_bTwoLineWinner(TRUE)
	, m_szLogoFilename(_T(""))
	, m_szAdText(_T(""))
	, m_szAdFilename(_T(""))
{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nCalledCount = 0;
	m_bStopped = false;
	m_nMatchedPanelCnt = 0;
	m_nLastMatchedPanelCnt = 0;
	m_nFinalWinnersCnt = 0;
	m_nFromPanel = 1;
	m_nToPanel = 100;
	m_nVerifiedCnt = 0;
	m_nCurrMatchLevel = 1;
	m_bNumberVerified = false;
	memset(m_npCalledNumbers, 0, sizeof(m_npCalledNumbers));
	memset(m_nppWinnerPanelInfo, 0, sizeof(m_nppWinnerPanelInfo));
	g_pCallerDlg = this;
	m_bCanStartNewGameWithNumber = false;
	m_userScreen = 0;
	m_nSessionCnt = 0;
	m_nCurrSessionId = 0;
}

CBingoCallerDlg::~CBingoCallerDlg()
{
	int kk = 0;
}

void trim(char* str)
{
	int len = strlen(str);
	while (str[len-1]==' '||str[len-1]=='\r'||str[len-1]=='\n')
	{
		str[len - 1] = 0;
		len--;
	}
	int start = 0;
	while (str[start]==' '||str[start]=='\t'||str[start]=='\r')
	{
		start++;
	}
	memmove(str, str + start, len - start);
}

int connectDB()
{
	FILE* fp = fopen("database.txt", "r");
// 	fread()
	char username[100], password[100];
	if (fp)
	{
		fgets(username, 100, fp); trim(username); 
		fgets(password, 100, fp); trim(password);
		fclose(fp);
		mysql = mysql_init(0);
		if (mysql && mysql_real_connect(mysql, "localhost", username, password, "bingo", 3306, NULL, 0) == NULL)
		{
			return -1;
		}

	}
	else
		return -1;
	return 0;
}
void initializeServer();
UINT connectToClient();


void CBingoCallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nPanelNumber);
	DDX_Text(pDX, IDC_EDIT_ONE_LINE, m_szOneLine);
	DDX_Text(pDX, IDC_EDIT_TWO_LINE, m_szTwoLine);
	DDX_Text(pDX, IDC_EDIT_FULL_HOUSE, m_szFullHouse);
	DDX_Text(pDX, IDC_EDIT_RANGE_FROM, m_nPanelRangeFrom);
	DDX_Text(pDX, IDC_EDIT_RANGE_TO, m_nPanelRangeTo);
	DDX_Text(pDX, IDC_EDIT_ONE_LINE_MISS, m_szOneLineMiss);
	DDX_Text(pDX, IDC_EDIT_TWO_LINE_MISS, m_szTwoLineMiss);
	DDX_Text(pDX, IDC_EDIT_FULL_HOUSE_MISS, m_szFullHouseMiss);
	DDX_Check(pDX, IDC_CHECK_ONELINE, m_bOneLineWinner);
	DDX_Check(pDX, IDC_CHECK_TWOLINE, m_bTwoLineWinner);
	DDX_Text(pDX, IDC_EDIT_LOGO_FILE, m_szLogoFilename);
	DDX_Text(pDX, IDC_EDIT_ADVERTISE_TEXT, m_szAdText);
	DDX_Text(pDX, IDC_EDIT_AD_FILE, m_szAdFilename);
	DDX_Control(pDX, IDC_COMBO_SESSION_NAME, m_comboSession);
}

BEGIN_MESSAGE_MAP(CBingoCallerDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CBingoCallerDlg::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDOK, &CBingoCallerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SET_RANGE, &CBingoCallerDlg::OnBnClickedButtonSetRange)
	ON_BN_CLICKED(IDC_CHECK_ONELINE, &CBingoCallerDlg::OnBnClickedCheckOneline)
	ON_BN_CLICKED(IDC_CHECK_TWOLINE, &CBingoCallerDlg::OnBnClickedCheckTwoline)
	ON_BN_CLICKED(IDC_BUTTON_LOGO, &CBingoCallerDlg::OnBnClickedButtonLogo)
	ON_BN_CLICKED(IDC_BUTTON_ADVERTISE, &CBingoCallerDlg::OnBnClickedButtonAdvertise)
	ON_BN_CLICKED(IDC_BUTTON_AD_TEXT_SET, &CBingoCallerDlg::OnBnClickedButtonAdTextSet)
	ON_EN_CHANGE(IDC_EDIT_RANGE_FROM, &CBingoCallerDlg::OnEnChangeEditRangeFrom)
	ON_EN_CHANGE(IDC_EDIT_RANGE_TO, &CBingoCallerDlg::OnEnChangeEditRangeTo)
	ON_BN_CLICKED(IDC_BUTTON_AD_IMAGE_REMOVE, &CBingoCallerDlg::OnBnClickedButtonAdImageRemove)
	ON_CBN_SELCHANGE(IDC_COMBO_SESSION_NAME, &CBingoCallerDlg::OnCbnSelchangeComboSessionName)
END_MESSAGE_MAP()


// CBingoCallerDlg message handlers

BOOL CBingoCallerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon

	connectDB();
	CRect rect;
	GetWindowRect(rect);
// 	SetWindowPos(&CWnd::wndTopMost, 0, 0, rect.Width(), rect.Height(), SWP_NOSIZE);
	CreateButtons();
// 	CBitmapButton*btn = (CBitmapButton*)GetDlgItem(IDC_BUTTON_CHECK);
	CButton*btn = (CButton*)GetDlgItem(IDC_BUTTON_CHECK);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_CHECK_BUTTON);
	btn->ModifyStyle(0, BS_BITMAP);
	btn->SetBitmap(bmp);
// 	btn->LoadBitmaps(IDB_BITMAP_CHECK_BUTTON);
// 	btn->SizeToContent();

	CRect rect2;
#ifdef TICKET_ONLEY
	CWnd* onelinecheck = GetDlgItem(IDC_CHECK_ONELINE);
	onelinecheck->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect2.top -= 20;
	rect2.bottom -= 20;
	onelinecheck->MoveWindow(rect2);
	CWnd* twolinecheck = GetDlgItem(IDC_CHECK_TWOLINE);
	twolinecheck->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect2.top -= 20;
	rect2.bottom -= 20;
	twolinecheck->MoveWindow(rect2);
#endif // TICKET_ONLEY

	CRect rect1;
	CWnd* pCallNumber = GetDlgItem(IDC_STATIC_NUMBERS_CALLED);
	pCallNumber->GetWindowRect(rect1);
	ScreenToClient(rect1);

	CWnd* pNumbersCalledShow = GetDlgItem(IDC_STATIC_NUMBERS_CALLED_SHOW);
	pNumbersCalledShow->GetWindowRect(m_calledCountRect);
	ScreenToClient(m_calledCountRect);

	CWnd* pCalledNumberShow = GetDlgItem(IDC_STATIC_CALLED_NUMBER_SHOW);
	pCalledNumberShow->GetWindowRect(m_LastCalledNumberRect);
	ScreenToClient(m_LastCalledNumberRect);

	CWnd* pNumbersPanel = GetDlgItem(IDC_STATIC_NUMBERS_PANEL);
	pNumbersPanel->GetWindowRect(m_nNumPanelRect);
	ScreenToClient(m_nNumPanelRect);
	NUMBER_PANEL_ITEM_WIDTH = (m_nNumPanelRect.Width() - NUMBER_PANEL_ITEM_INTERVAL * 1.5 * 2 - NUMBER_PANEL_ITEM_INTERVAL*2 - NUMBER_PANEL_ITEM_INTERVAL * (NUMBER_PANEL_COL_COUNT - 1))/NUMBER_PANEL_COL_COUNT;
	NUMBER_PANEL_ITEM_HEIGHT = (m_nNumPanelRect.Height() - NUMBER_PANEL_ITEM_INTERVAL * 2 - NUMBER_PANEL_ITEM_INTERVAL * 2 - NUMBER_PANEL_ITEM_INTERVAL * (90 / NUMBER_PANEL_COL_COUNT - 1)) / (90 / NUMBER_PANEL_COL_COUNT);
	makeNumberPanel();

// 	if (getPanelRange() == false)
// 	{
// 		AfxMessageBox(_T("Please select panel range on range.txt"));
// 		return FALSE;
// 	}
	setColorInfo();
	loadSessionNames();
// 	getPanelRangeInfo();
// 	loadPanelInfo();
	m_nGameNumber = 1;
// 	AfxBeginThread((AFX_THREADPROC)connectToClient, 0, THREAD_PRIORITY_NORMAL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBingoCallerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetDlgItem(IDC_STATIC_COLOR_PAD)->GetWindowRect(rect);
	ScreenToClient(rect);
	if (PtInRect(rect, point))
	{
		CSelectColorDlg colorDlg;
		if (colorDlg.DoModal() == IDOK)
		{
			if (MessageBox(_T("This setting will be applied after start new game. Do you want to continue?"), _T("Query"), MB_OKCANCEL) == IDCANCEL)
				return;
			_tcscpy_s(m_currColorStr, colorDlg.m_szCurrColor);
			CWnd* pColorPadWnd = GetDlgItem(IDC_STATIC_COLOR_PAD);
			pColorPadWnd->GetWindowRect(rect);
			ScreenToClient(rect);
			InvalidateRect(rect);
		}
	}
}

void CBingoCallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBingoCallerDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBingoCallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBingoCallerDlg::CreateButtons()
{
	CRect rect;

	GetWindowRect(rect);
	CWnd* pAction = GetDlgItem(IDC_STATIC_ACTIONS);
	CRect rect1;
	pAction->GetWindowRect(rect1);
	CRect rect2;
	CRect action_rect;
	ScreenToClient(rect1);
	action_rect = rect1;
	int call_button_size = action_rect.Height() - 10;
	int nButtonWidth = call_button_size-40;
	rect2.left = action_rect.left + 40;
	rect2.top = action_rect.top + (action_rect.Height() - nButtonWidth) / 2 - 5;
	rect2.right = rect2.left + nButtonWidth;
	rect2.bottom = rect2.top + nButtonWidth;
	m_cStartButtonControl.Create(this, rect2, 0);
	m_cStartButtonControl.m_nID = 0;
	m_cStartButtonControl.m_nBitmap = IDB_BITMAP_BLUE1;
	m_cStartButtonControl.m_nClickedBitmap = IDB_BITMAP_RED1;
	_tcscpy_s(m_cStartButtonControl.m_image_path1, _T("./res/blueButton_start.jpg"));
	_tcscpy_s(m_cStartButtonControl.m_image_path2, _T("./res/redButton_start.jpg"));
	_tcscpy_s(m_cStartButtonControl.m_str, _T("Start Game"));
	CRgn rgn;
	rgn.CreateEllipticRgn(BALL_BUTTON_CUT_MARGIN, BALL_BUTTON_CUT_MARGIN, nButtonWidth - BALL_BUTTON_CUT_MARGIN, nButtonWidth - BALL_BUTTON_CUT_MARGIN);
	m_cStartButtonControl.SetWindowRgn(rgn, TRUE);

	rect2.left = rect2.right + 5;
	rect2.right = rect2.left + nButtonWidth;
	rect2.top = rect2.top;
	rect2.bottom = rect2.top + nButtonWidth;
	m_cStopButtonControl.Create(this, rect2, 1);
	m_cStopButtonControl.m_nID = 1;
	m_cStopButtonControl.m_nBitmap = IDB_BITMAP_BLUE1;
	m_cStopButtonControl.m_nClickedBitmap = IDB_BITMAP_RED1;
	_tcscpy_s(m_cStopButtonControl.m_image_path1, _T("./res/blueButton_stop.jpg"));
	_tcscpy_s(m_cStopButtonControl.m_image_path2, _T("./res/redButton_stop.jpg"));
	_tcscpy_s(m_cStopButtonControl.m_str, _T("Stop Game"));
	CRgn rgn1;
	rgn1.CreateEllipticRgn(BALL_BUTTON_CUT_MARGIN, BALL_BUTTON_CUT_MARGIN, nButtonWidth - BALL_BUTTON_CUT_MARGIN, nButtonWidth - BALL_BUTTON_CUT_MARGIN);
	m_cStopButtonControl.SetWindowRgn(rgn1, TRUE);

	rect2.left = action_rect.left + action_rect.Width()/2 - call_button_size/2;
	rect2.right = rect2.left + call_button_size;
	rect2.top = action_rect.top + (action_rect.Height() - call_button_size) / 2 + 5;
	rect2.bottom = rect2.top + call_button_size;
	m_cCallButtonControl.Create(this, rect2, 2);
	m_cCallButtonControl.m_nID = 2;
	m_cCallButtonControl.m_nBitmap = IDB_BITMAP_RED;
	m_cCallButtonControl.m_nClickedBitmap = IDB_BITMAP_GOLD;
	_tcscpy_s(m_cCallButtonControl.m_image_path1, _T("./res/redButton_call.jpg"));
	_tcscpy_s(m_cCallButtonControl.m_image_path2, _T("./res/goldButton_call.jpg"));
	_tcscpy_s(m_cCallButtonControl.m_str, _T("CALL"));
	CRgn rgn2;
	rgn2.CreateEllipticRgn(BALL_BUTTON_CUT_MARGIN, BALL_BUTTON_CUT_MARGIN, call_button_size - BALL_BUTTON_CUT_MARGIN, call_button_size - BALL_BUTTON_CUT_MARGIN);
	m_cCallButtonControl.SetWindowRgn(rgn2, TRUE);

	rect2.right = action_rect.right - 40;
	rect2.left = rect2.right - nButtonWidth;
	rect2.top = action_rect.top + (action_rect.Height() - nButtonWidth) / 2 - 5;
	rect2.bottom = rect2.top + nButtonWidth;
	m_cVerifyButtonControl.Create(this, rect2, 3);
	m_cVerifyButtonControl.m_nID = 3;
	m_cVerifyButtonControl.m_nBitmap = IDB_BITMAP_GOLD1;
	m_cVerifyButtonControl.m_nClickedBitmap = IDB_BITMAP_RED1;
	_tcscpy_s(m_cVerifyButtonControl.m_image_path1, _T("./res/goldButton_verify.jpg"));
	_tcscpy_s(m_cVerifyButtonControl.m_image_path2, _T("./res/redButton_verify.jpg"));
	_tcscpy_s(m_cVerifyButtonControl.m_str, _T("Verify"));
	CRgn rgn3;
	rgn3.CreateEllipticRgn(BALL_BUTTON_CUT_MARGIN, BALL_BUTTON_CUT_MARGIN, nButtonWidth - BALL_BUTTON_CUT_MARGIN, nButtonWidth - BALL_BUTTON_CUT_MARGIN);
	m_cVerifyButtonControl.SetWindowRgn(rgn3, TRUE);

	rect2.right = rect2.left - 5;
	rect2.left = rect2.right - nButtonWidth;
	rect2.top = rect2.top;
	rect2.bottom = rect2.top + nButtonWidth;
	m_cRecallButtonControl.Create(this, rect2, 3);
	m_cRecallButtonControl.m_nID = 4;
	m_cRecallButtonControl.m_nBitmap = IDB_BITMAP_GOLD1;
	m_cRecallButtonControl.m_nClickedBitmap = IDB_BITMAP_RED1;
	_tcscpy_s(m_cRecallButtonControl.m_image_path1, _T("./res/goldButton_recall.jpg"));
	_tcscpy_s(m_cRecallButtonControl.m_image_path2, _T("./res/redButton_recall.jpg"));
	_tcscpy_s(m_cRecallButtonControl.m_str, _T("Recall"));
	CRgn rgn4;
	rgn4.CreateEllipticRgn(BALL_BUTTON_CUT_MARGIN, BALL_BUTTON_CUT_MARGIN, nButtonWidth - BALL_BUTTON_CUT_MARGIN, nButtonWidth - BALL_BUTTON_CUT_MARGIN);
	m_cRecallButtonControl.SetWindowRgn(rgn4, TRUE);



	CWnd* pCallNumber = GetDlgItem(IDC_STATIC_CALL_NUMBER);
	pCallNumber->GetWindowRect(rect1);
	ScreenToClient(rect1);

	rect2.left = rect1.left + 20;
	rect2.right = rect1.right - 20;
	rect2.top = rect1.top + 60;
	rect2.bottom = rect2.top + rect2.Width();
	
}


void CBingoCallerDlg::OnDraw(CDC* dc)
{
	drawYellowBoxes(dc);
	HPEN pen;
// 	pen = CreatePen(PS_SOLID, 3, RGB(50, 50, 50));
// 	SelectObject(dc->m_hDC, pen);
// 	dc->Rectangle(m_calledCountRect);

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
	_stprintf_s(m_szNumberCount, _T("%d"), m_nCalledCount);
	SetBkMode(dc->m_hDC, TRANSPARENT);
	dc->SetTextColor(RGB(255, 255, 255));
	DrawText(dc->m_hDC, m_szNumberCount, -1, m_calledCountRect, DT_CENTER |DT_VCENTER | DT_SINGLELINE);
	lf.lfHeight = 280;
	font3.CreateFontIndirect(&lf);

	if(m_nCalledLastNumber > 0)
		_stprintf_s(m_szNumberCount, _T("%d"), m_nCalledLastNumber);
	else
		_stprintf_s(m_szNumberCount, _T(""));
	dc->SelectObject(&font3);
	dc->SetTextColor(RGB(0, 0, 0));
	CRect rect1;
	rect1.left = m_LastCalledNumberRect.left; rect1.top = m_LastCalledNumberRect.top; rect1.right = m_LastCalledNumberRect.right + 4; rect1.bottom = m_LastCalledNumberRect.bottom + 4;
// 	dc->BeginPath();
	DrawText(dc->m_hDC, m_szNumberCount, -1, rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
// 	dc->EndPath();
// 	dc->StrokeAndFillPath();
	dc->SelectObject(&font);
	dc->SetTextColor(RGB(234, 34, 24));
	DrawText(dc->m_hDC, m_szNumberCount, -1, m_LastCalledNumberRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	

	pen = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
	SelectObject(dc->m_hDC, pen);

	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 40; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));
	font1.CreateFontIndirect(&lf);
	dc->SelectObject(&font1);

	COLORREF clr = RGB(0, 103, 177);
	CBrush br1, br2, br3, *br;
	br1.CreateSolidBrush(clr);
	br2.CreateSolidBrush(RGB(234, 34, 24));
	br3.CreateSolidBrush(RGB(61, 226, 159));
	CBrush* oldBrush = dc->SelectObject(&br1);
	TCHAR str_num[3];
	dc->SetTextColor(RGB(255, 255, 255));
	for (int i = 0; i < 90/ NUMBER_PANEL_COL_COUNT; i++)
	{
		for (int j = 0; j < NUMBER_PANEL_COL_COUNT; j++)
		{
			if (g_number_panel[i][j].bCalled && g_number_panel[i][j].number == m_nCalledLastNumber)
				br = &br3;
			else if (g_number_panel[i][j].bCalled)
				br = &br2;
			else
				br = &br1;
			dc->SelectObject(br);
// 			dc->FillRect(g_number_panel[i][j].rect, br);
			dc->Rectangle(g_number_panel[i][j].rect);
			_stprintf_s(str_num, _T("%d"), g_number_panel[i][j].number);
			DrawText(dc->m_hDC, str_num, -1, g_number_panel[i][j].rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	dc->SelectObject(&oldfont);
	dc->SelectObject(oldBrush);
	font.DeleteObject();
	DeleteObject(pen);
}


LRESULT CBingoCallerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_START)
	{
		if (m_nCurrSessionId == 0) {
			AfxMessageBox(_T("Select session."));
			return 0;
		}
		if (m_bNumberVerified)
		{
			if (MessageBox(_T("Are you going to stop this game?"), _T("Query"), MB_OKCANCEL) == IDOK)
				SendMessage(WM_STOP);
			else
				return 0;
		}
		UpdateData(TRUE);
		m_nFromPanel = m_nPanelRangeFrom;
		m_nToPanel = m_nPanelRangeTo;
		resetNumberArray();
		resetPanelInfo();
		m_userScreen->resetPanel();
		m_bStopped = false;
		m_nCalledCount = 0;
		resetCalledNumbersArray();
		m_nCalledLastNumber = 0;
		CRect rect;
		rect.left = g_nNumberPanelMargin;
		InvalidateRect(m_LastCalledNumberRect);
		InvalidateRect(m_nNumPanelRect);
		InvalidateRect(m_calledCountRect);
// 		m_numPanel.m_szNumber[0] = 0;
		m_nMatchedPanelCnt = 0;
		m_nLastMatchedPanelCnt = 0;
		m_nFinalWinnersCnt = 0;
// 		m_numPanel.Invalidate(TRUE);
		m_szOneLineMiss.Format(_T(""));
		m_szTwoLineMiss.Format(_T(""));
		m_szFullHouseMiss.Format(_T(""));
		m_nVerifiedCnt = 0;
		m_bNumberVerified = false;
		m_szOneLine.Format(_T(""));
		m_szTwoLine.Format(_T(""));
		m_szFullHouse.Format(_T(""));
		UpdateData(FALSE);
		m_nPanelNumber = 0;
		m_nCurrMatchLevel = 1;
		m_onelineMissedInfo.cnt = 0;
		m_twolineMissedInfo.cnt = 0;
		m_fullhouseMissedInfo.cnt = 0;
		m_onelineMatchedInfo.RemoveAll();
		m_twolineMatchedInfo.RemoveAll();
		m_fullhouseMatchedInfo.RemoveAll();
		m_VerifiedInfo.cnt = 0;
		g_nMatchedPanelCnt = 0;

// 		getPanelRangeInfo();
		loadPanelInfo();
		g_recall_info.RemoveAll();
		g_nRecallCount = 0;
		if (m_bCanStartNewGameWithNumber)
		{
			CWnd* pGameNumberPadWnd = GetDlgItem(IDC_STATIC_GAME_NUMBER_PAD);
			pGameNumberPadWnd->GetWindowRect(rect);
			ScreenToClient(rect);
			InvalidateRect(rect);
			m_nGameNumber++;
			m_bCanStartNewGameWithNumber = false;
		}
		resetUserScreen();
		UpdateData(FALSE);
	}
	else if (message == WM_STOP)
	{
		if (MessageBox(_T("Are you sure you want to stop this game?"), _T("Question"), MB_YESNO) == IDYES)
		{
			m_bStopped = true;
			if (m_bNumberVerified && g_nRecallCount == 0 && g_nMatchedPanelCnt > 0)
			{
				moveUnverifiedPanelsToMissedMatch();
				copyVerifiedInfo(m_nCurrMatchLevel);
				rearrangeVerifiedArrayInfo();
				TCHAR str[10000], ss[10000]; str[0] = 0, ss[0] = 0;
				makeWinnerString(ss);
				_stprintf_s(str, _T("Game finished! Final winners are \r\n%s"), ss);
				MessageBox(str, _T("Notification"), MB_OK);
				m_bCanStartNewGameWithNumber = true;
				return 0;
			}
			resetPanelInfo();
		}
	}
	else if (message == WM_CALL)
	{
		if (m_nCurrSessionId == 0) {
			AfxMessageBox(_T("Select session."));
			return 0;
		}
		CallAction();
	}
	else if (message == WM_RECALL)
	{
		RecallAction();
	}
	else if (message == WM_VERIFY)
	{
		verifyLastMatched();
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


HBRUSH CBingoCallerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

// 	if (nCtlColor == CTLCOLOR_STATIC)
// 	{
// 		pDC->SetBkColor(RGB(185, 212, 250));
// 	}
	if (pWnd->GetDlgCtrlID() == IDC_CHECK_ONELINE || pWnd->GetDlgCtrlID()==IDC_CHECK_TWOLINE)
	{
		pDC->SetBkColor(RGB(255, 196, 37));
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 196, 37));
		return (HBRUSH)brush;
	}
	return hbr;
}


void CBingoCallerDlg::makeNumberPanel()
{
// 	CWnd* pAction = GetDlgItem(IDC_STATIC_ACTIONS);
// 	CRect rect1;
// 	pAction->GetWindowRect(rect1);
// 	ScreenToClient(rect1);
// 	int top = rect1.bottom+g_nNumberPanelTopMargin;
	for (int i = 0; i < 90/ NUMBER_PANEL_COL_COUNT; i++)
	{
		for (int j = 0; j < NUMBER_PANEL_COL_COUNT; j++)
		{
			g_number_panel[i][j].number = i * NUMBER_PANEL_COL_COUNT + j + 1;
			g_number_panel[i][j].rect.left = m_nNumPanelRect.left + 2.5*NUMBER_PANEL_ITEM_INTERVAL + j * (NUMBER_PANEL_ITEM_WIDTH + NUMBER_PANEL_ITEM_INTERVAL);
			g_number_panel[i][j].rect.right = g_number_panel[i][j].rect.left + NUMBER_PANEL_ITEM_WIDTH;
			g_number_panel[i][j].rect.top = m_nNumPanelRect.top + 2*NUMBER_PANEL_ITEM_INTERVAL + i * NUMBER_PANEL_ITEM_HEIGHT + i * NUMBER_PANEL_ITEM_INTERVAL;
			g_number_panel[i][j].rect.bottom = g_number_panel[i][j].rect.top + NUMBER_PANEL_ITEM_HEIGHT;
			g_number_panel[i][j].bCalled = false;
		}
	}
// 	m_nNumPanelRect.left = g_nNumberPanelMargin;
// 	m_nNumPanelRect.right = m_nNumPanelRect.left + NUMBER_PANEL_COL_COUNT * NUMBER_PANEL_ITEM_WIDTH + (NUMBER_PANEL_COL_COUNT-1) * NUMBER_PANEL_ITEM_INTERVAL;
// 	m_nNumPanelRect.top = top;
// 	m_nNumPanelRect.bottom = top + (90/NUMBER_PANEL_COL_COUNT) * NUMBER_PANEL_ITEM_HEIGHT + (90/NUMBER_PANEL_COL_COUNT-1) * NUMBER_PANEL_ITEM_INTERVAL;
}


void CBingoCallerDlg::resetNumberArray()
{
	for (int i = 0; i < 90/NUMBER_PANEL_COL_COUNT; i++)
	{
		for (int j = 0; j < NUMBER_PANEL_COL_COUNT; j++)
		{
			g_number_panel[i][j].bCalled = false;
		}
	}
}


int CBingoCallerDlg::resetCalledNumbersArray()
{
	memset(m_npCalledNumbers, 0, sizeof(m_npCalledNumbers));
	return 0;
}


bool CBingoCallerDlg::checkInCalledNumbers(int num)
{
	for (int i = 0; i < m_nCalledCount; i++)
	{
		if (num == m_npCalledNumbers[i])
			return true;
	}
	return false;
}

bool checkInNumberArr(int* arr, int cnt, int val)
{
	for (int i = 0; i < cnt; i++)
	{
		if (arr[i] == val)
			return true;
	}
	return false;
}

void CBingoCallerDlg::OnBnClickedButtonCheck()
{
	UpdateData(TRUE);
	char query[1000];
	int nTableId = m_nPanelNumber % 6;
	if (nTableId == 0)
		nTableId = 6;
// 	sprintf_s(query, "select panel_1_copy.panel_id, panel_1_copy.numbers, panel_2_copy.panel_id, panel_2_copy.numbers, panel_3_copy.panel_id, panel_3_copy.numbers, panel_4_copy.panel_id, panel_4_copy.numbers, panel_5_copy.panel_id, panel_5_copy.numbers,\
// 		panel_6_copy.panel_id, panel_6_copy.numbers from panel_1_copy, panel_2_copy, panel_3_copy, panel_4_copy, panel_5_copy, panel_6_copy where panel_%d_copy.panel_id='%d' and panel_%d_copy.card_id=panel_1_copy.card_id and panel_%d_copy.card_id=panel_2_copy.card_id \
// 		and panel_%d_copy.card_id=panel_3_copy.card_id and panel_%d_copy.card_id=panel_4_copy.card_id and panel_%d_copy.card_id=panel_5_copy.card_id and panel_%d_copy.card_id=panel_6_copy.card_id", nTableId, m_nPanelNumber, nTableId,
// 		nTableId, nTableId, nTableId, nTableId, nTableId);
	sprintf_s(query, "select panel_1.panel_id, panel_1.numbers, panel_2.panel_id, panel_2.numbers, panel_3.panel_id, panel_3.numbers, panel_4.panel_id, panel_4.numbers, panel_5.panel_id, panel_5.numbers,\
		panel_6.panel_id, panel_6.numbers from panel_1, panel_2, panel_3, panel_4, panel_5, panel_6 where panel_%d.panel_id='%d' and panel_%d.card_id=panel_1.card_id and panel_%d.card_id=panel_2.card_id \
		and panel_%d.card_id=panel_3.card_id and panel_%d.card_id=panel_4.card_id and panel_%d.card_id=panel_5.card_id and panel_%d.card_id=panel_6.card_id", nTableId, m_nPanelNumber, nTableId,
		nTableId, nTableId, nTableId, nTableId, nTableId);
	if (mysql_query(mysql, query) == 0)
	{
		res = mysql_store_result(mysql);
		if (res->row_count == 1)
		{
			row = mysql_fetch_row(res);
			writeToWinnerPanelInfo(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11]);
			CPanelViewDlg dlg(this);
			dlg.m_nPanelId = m_nPanelNumber;

			dlg.DoModal();
			if (int res = checkFullHouse(nTableId - 1) > 0)
			{
				if (checkInNumberArr(m_npMatchedPanels, m_nMatchedPanelCnt, m_nPanelNumber) == false && m_nMatchedPanelCnt<50)
				{
					m_npMatchedPanels[m_nMatchedPanelCnt++] = m_nPanelNumber;
					UpdateData(FALSE);
				}
			}
		}
		else if (res->row_count == 0)
		{
			MessageBox(_T("Invalid panel serial number!"), _T("Caution"), MB_OK);
			mysql_free_result(res);
			return;
		}
		mysql_free_result(res);
	}
}


void CBingoCallerDlg::writeToWinnerPanelInfo(char* panel_id1, char* panel_numbers1, char* panel_id2, char* panel_numbers2, char* panel_id3, char* panel_numbers3, char* panel_id4, char* panel_numbers4, char* panel_id5, char* panel_numbers5, char* panel_id6, char* panel_numbers6)
{
	m_npWinnerCardSerialInfo[0] = atoi(panel_id1);
	m_npWinnerCardSerialInfo[1] = atoi(panel_id2);
	m_npWinnerCardSerialInfo[2] = atoi(panel_id3);
	m_npWinnerCardSerialInfo[3] = atoi(panel_id4);
	m_npWinnerCardSerialInfo[4] = atoi(panel_id5);
	m_npWinnerCardSerialInfo[5] = atoi(panel_id6);
	int idx = 0;
	for (int panel_id = 0; panel_id < 6; panel_id++)
	{
		char* str;
		if (panel_id == 0)
			str = panel_numbers1;
		else if (panel_id == 1)
			str = panel_numbers2;
		else if (panel_id == 2)
			str = panel_numbers3;
		else if (panel_id == 3)
			str = panel_numbers4;
		else if (panel_id == 4)
			str = panel_numbers5;
		else
			str = panel_numbers6;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 9; i++)
			{
				char ch[3];
				strncpy_s(ch, str, 2);
				ch[2] = 0;
				str += 2;
				m_nppWinnerPanelInfo[panel_id][i][j] = atoi(ch);
			}
		}
	}
}


int CBingoCallerDlg::checkFullHouse(int panel_id)
{
	int one_line_cnt = 0;
	int two_line_cnt = 0;
	int third_line_cnt = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_nppWinnerPanelInfo[panel_id][i][j] != 0)
			{
				if (checkInCalledNumbers(m_nppWinnerPanelInfo[panel_id][i][j]))
				{
					if (j == 0)
						one_line_cnt++;
					else if (j == 1)
						two_line_cnt++;
					else
						third_line_cnt++;
				}
			}
		}
	}
	if (one_line_cnt < 5 && two_line_cnt < 5 && third_line_cnt < 5)
		return 0;
	if ((one_line_cnt < 5 && two_line_cnt < 5) || (two_line_cnt < 5 && third_line_cnt < 5) || (third_line_cnt < 5 && one_line_cnt < 5))
		return ONE_LINE;
	else if (one_line_cnt < 5 || two_line_cnt < 5 || third_line_cnt < 5)
		return TWO_LINE;
	else
		return FULL_HOUSE;
}

char* CStringToChar(CString str, char* charStr)
{
	int len = str.GetLength();
	WideCharToMultiByte(CP_ACP, 0, str, -1, charStr, len, NULL, NULL);
	charStr[len] = 0;
	return charStr;
}

TCHAR* CharToTCHAR(char* charStr, TCHAR* str)
{
	int len = strlen(charStr);
	MultiByteToWideChar(CP_ACP, 0, charStr, len, str, len);
	str[len] = 0;
	return str;
}

int CStringToInt(CString str)
{
	char ss[20];
	CStringToChar(str, ss);
	return atoi(ss);
}

bool CBingoCallerDlg::verifyLastMatched()
{
	UpdateData(TRUE);
	char query[1000];
	int idx = 0;
	CString curr_str;
	if (m_nCurrMatchLevel == 1)
		curr_str = m_szOneLine;
	else if (m_nCurrMatchLevel == 2)
		curr_str = m_szTwoLine;
	else if (m_nCurrMatchLevel == 3)
		curr_str = m_szFullHouse;
	int len = curr_str.GetLength();
	int nNumber = m_nPanelNumber;
	int nTableId = nNumber % 6;
	if (nTableId == 0)
		nTableId = 6;
	sprintf_s(query, "select panel_1.panel_id, panel_1.numbers, panel_2.panel_id, panel_2.numbers, panel_3.panel_id, panel_3.numbers, panel_4.panel_id, panel_4.numbers, panel_5.panel_id, panel_5.numbers,\
		panel_6.panel_id, panel_6.numbers from panel_1, panel_2, panel_3, panel_4, panel_5, panel_6 where panel_%d.panel_id='%d' and panel_%d.card_id=panel_1.card_id and panel_%d.card_id=panel_2.card_id \
		and panel_%d.card_id=panel_3.card_id and panel_%d.card_id=panel_4.card_id and panel_%d.card_id=panel_5.card_id and panel_%d.card_id=panel_6.card_id", nTableId, nNumber, nTableId,
		nTableId, nTableId, nTableId, nTableId, nTableId);
	if (mysql_query(mysql, query) == 0)
	{
		res = mysql_store_result(mysql);
		if (res->row_count == 1)
		{
			row = mysql_fetch_row(res);
			writeToWinnerPanelInfo(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11]);
			TCHAR customer[100];
			getCustomerName(m_nCurrSessionId, m_nPanelNumber, customer);
			CPanelViewDlg dlg(this);
			_tcscpy_s(dlg.m_CustomerName, customer);
			dlg.m_nPanelId = nNumber;
			dlg.DoModal();
			int res = 0;
			if ((res = checkFullHouse(nTableId-1)) > 0)
			{
				if (res == m_nCurrMatchLevel)
					m_bNumberVerified = true;
				if (m_bNumberVerified)
				{
					checkAsVerified(nNumber);
					m_userScreen->m_bVerified = true;
					m_userScreen->m_nverifiedPanelId = nNumber;
					for (int i = 0; i < 9; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							m_userScreen->m_verifiedPanel[i][j] = m_nppWinnerPanelInfo[nTableId - 1][i][j];
						}
					}
// 					m_userScreen->addToVerified(nNumber, m_nCurrMatchLevel);
					m_userScreen->InvalidateRect(m_userScreen->m_LogoPanelRect);
				}
			}
		}
		else if (res->row_count == 0)
		{
			MessageBox(_T("Invalid panel serial number!"), _T("Caution"), MB_OK);
			mysql_free_result(res);
			return false;
		}
		mysql_free_result(res);
	}
	return false;
}


bool CBingoCallerDlg::checkLastNumberInWinningPanel(int panel_id)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (m_nppWinnerPanelInfo[panel_id][i][j] == m_nCalledLastNumber)
				return true;
		}
	}
	return false;
}


void CBingoCallerDlg::OnBnClickedOk()
{

}


bool CBingoCallerDlg::getPanelRange() //depricated
{
	FILE* fp = fopen("range.txt", "r");
	if (fp)
	{
		char from[100];
		char to[100];
		fgets(from, 100, fp); trim(from);
		fgets(to, 100, fp); trim(to);
		int nFromPanel = atoi(from);
		int nToPanel = atoi(to);
		if (nFromPanel == 0 || nToPanel == 0)
		{
			m_nFromPanel = 0;
			m_nToPanel = 0;
			return false;
		}
		if ((nToPanel < nFromPanel)||(nToPanel-nFromPanel+1> MAX_RANGE_COUNT))
		{
			if (nFromPanel > nToPanel)
			{
				AfxMessageBox(_T("Invalid range"));
			}
			else if ((nToPanel - nFromPanel + 1) > MAX_RANGE_COUNT)
			{
				TCHAR str[100];
				_stprintf_s(str, _T("The panel range must be less than %d"), MAX_RANGE_COUNT);
				AfxMessageBox(str);
			}
			m_nFromPanel = 0;
			m_nToPanel = 0;
			return false;
		}
		m_nFromPanel = nFromPanel;
		m_nToPanel = nToPanel;
	}
	else
	{
		m_nFromPanel = 0;
		m_nToPanel = 0;
		return false;
	}
	return true;
}


void CBingoCallerDlg::getPanelRangeInfo()
{
	char query[300];
	g_nPanelInfoCnt = 0;
	for (int i = 1; i <= 6; i++)
	{
		sprintf_s(query, "select *from panel_%d where panel_id>='%d' and panel_id<='%d'", i, m_nFromPanel, m_nToPanel);
		if (mysql_query(mysql, query) == 0)
		{
			res = mysql_store_result(mysql);
			while (row = mysql_fetch_row(res))
			{
				g_panel_info[g_nPanelInfoCnt].panel_id = atoi(row[1]);
				char* str = row[2];
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 9; k++)
					{
						char ch[3];
						strncpy_s(ch, str, 2);
						ch[2] = 0;
						str += 2;
						g_panel_info[g_nPanelInfoCnt].items[k][j].val = atoi(ch);
						g_panel_info[g_nPanelInfoCnt].items[k][j].bmatch = false;
					}
				}
				g_nPanelInfoCnt++;
			}
		}
	}
}


void CBingoCallerDlg::resetPanelInfo()
{
	for (int i = 0; i < MAX_RANGE_COUNT; i++)
	{
		g_panel_info[i].panel_id = 0;
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				g_panel_info[i].items[k][j].val = 0;
				g_panel_info[i].items[k][j].bmatch = false;
			}
		}
	}
	g_nPanelInfoCnt = 0;
}


int CBingoCallerDlg::checkPanelInfo(int number, int level)
{
	int match_line_cnt = 0;
	g_nMatchedPanelCnt = 0;
	for (int i = 0; i < g_nPanelInfoCnt; i++)
	{
		int match_lines = 0;
		bool bMatched = false;
		for (int j = 0; j < 3; j++)
		{
			int match_cnt = 0;
			bool bLastNumberMatched = false;
			for (int k = 0; k < 9; k++)
			{
				if (g_panel_info[i].items[k][j].val == number)
				{
					bLastNumberMatched = true;
					g_panel_info[i].items[k][j].bmatch = true;
				}
				if (g_panel_info[i].items[k][j].bmatch)
					match_cnt++;
			}
			if (match_cnt == 5)
				match_lines++;
			if (match_cnt == 5 && bLastNumberMatched)
				bMatched = true;
		}
		if (bMatched && match_lines == level)
		{
			g_npMatchedPanels[g_nMatchedPanelCnt++] = g_panel_info[i].panel_id;
			match_line_cnt++;
		}
	}
	return match_line_cnt;
}

#define RECALL_COUNT 89

void CBingoCallerDlg::recordCallingAction(st_recall_info recall_info)
{
	if (g_recall_info.GetCount() > RECALL_COUNT)
		g_recall_info.RemoveAt(0, 1);
	g_recall_info.Add(recall_info);
}


void CBingoCallerDlg::RecallAction()
{
	if (g_nRecallCount >= RECALL_COUNT)
		return;
	int cnt = g_recall_info.GetCount();
	if (g_nRecallCount+1 >= cnt)
		return;
// 	int nPrevLastCalledNumber = m_nCalledLastNumber;
	g_nRecallCount++;
	st_recall_info recall_info = g_recall_info.GetAt(cnt - g_nRecallCount -1);
	m_nCalledCount--;
	g_number_panel[(m_nCalledLastNumber - 1) / NUMBER_PANEL_COL_COUNT][(m_nCalledLastNumber - 1) % NUMBER_PANEL_COL_COUNT].bCalled = false;
	InvalidateRect(g_number_panel[(m_nCalledLastNumber - 1) / NUMBER_PANEL_COL_COUNT][(m_nCalledLastNumber - 1) % NUMBER_PANEL_COL_COUNT].rect);
// 	InvalidateRect(g_number_panel[(nPrevLastCalledNumber - 1) / NUMBER_PANEL_COL_COUNT][(nPrevLastCalledNumber - 1) % NUMBER_PANEL_COL_COUNT].rect);
	m_nCalledLastNumber = recall_info.called_number;
	InvalidateRect(g_number_panel[(m_nCalledLastNumber - 1) / NUMBER_PANEL_COL_COUNT][(m_nCalledLastNumber - 1) % NUMBER_PANEL_COL_COUNT].rect);
	// 	_stprintf_s(m_numPanel.m_szNumber, _T("%d"), m_nCalledLastNumber);
// 	m_numPanel.Invalidate(TRUE);
	InvalidateRect(m_calledCountRect);
	InvalidateRect(m_LastCalledNumberRect);
}


bool CBingoCallerDlg::CallAction()
{
	if (m_nToPanel == 0 && m_nFromPanel == 0)
	{
		AfxMessageBox(_T("Please select panel range"));
		// 			AfxMessageBox(_T("Please select panel range on range.txt"));
		return 0;
	}
// 	if (m_nCurrMatchLevel == 4 && m_bNumberVerified == false && g_nRecallCount == 0)
// 	{
// 		MessageBox(_T("Please verify full house winner"), _T("Notification!"), MB_OK);
// 		return 0;
// 	}
	if (m_bStopped && g_nRecallCount == 0)
	{
		MessageBox(_T("Calling stopped! If you want to call, start new game by clicking start button."), _T("Notification"), MB_OK);
		return 0;
	}
	if (m_nCalledCount >= 90)
	{
		MessageBox(_T("All numbers selected!"), _T("Warning"), MB_OK);
		return 0;
	}
	if (m_bNumberVerified == false && g_nRecallCount == 0 && g_nMatchedPanelCnt > 0)
	{
		if (MessageBox(_T("You have to verify matched number. \r\nOr are you going to continue current winning pattern?"), _T("Notification!"), MB_YESNO) == IDNO)
			return 0;
		moveUnverifiedPanelsToMissedMatch();
	}
	else if (m_bNumberVerified && m_nCurrMatchLevel < 3 && g_nRecallCount == 0 && g_nMatchedPanelCnt > 0)
	{
		if (m_nVerifiedCnt == g_nMatchedPanelCnt)
		{
			if (m_nCurrMatchLevel < 3 && MessageBox(_T("All matched numbers verified. \r\n Are you going to move to the next winning pattern?"), _T("Query"), MB_YESNO) == IDNO)
				return 0;
			copyVerifiedInfo(m_nCurrMatchLevel);
			m_nCurrMatchLevel++;
			m_userScreen->m_nGameLevel = m_nCurrMatchLevel;
			m_userScreen->InvalidateRect(m_userScreen->m_GameLevelRect);
		}
		if (m_nVerifiedCnt < g_nMatchedPanelCnt)
		{
			if (MessageBox(_T("Are you going to move to the next winning pattern?"), _T("Query"), MB_YESNO) == IDNO)
				return 0;
			moveUnverifiedPanelsToMissedMatch();
			copyVerifiedInfo(m_nCurrMatchLevel);
			m_nCurrMatchLevel++;
			m_userScreen->m_nGameLevel = m_nCurrMatchLevel;
			m_userScreen->InvalidateRect(m_userScreen->m_GameLevelRect);
		}
	}
	else if (m_bNumberVerified && m_nCurrMatchLevel == 3 && g_nRecallCount == 0 && g_nMatchedPanelCnt > 0)
	{
		if (MessageBox(_T("Matched numbers verified. \r\n Are you going to stop this game?"), _T("Query"), MB_YESNO) == IDNO)
			return 0;
		else
		{
			SendMessage(WM_STOP, 0, 0);
			return 0;
		}
	}
	if (g_nRecallCount == 0)
	{
		if (!m_bOneLineWinner && m_nCurrMatchLevel == 1)
			m_nCurrMatchLevel++;
		if (!m_bTwoLineWinner && m_nCurrMatchLevel == 2)
			m_nCurrMatchLevel++;
		g_nMatchedPanelCnt = 0;
		int nPrevLastCalledNumber = m_nCalledLastNumber;
		time_t curr_time;
		time(&curr_time);
		do
		{
			m_nCalledLastNumber = (rand() + curr_time) % 90 + 1;
			m_npCalledNumbers[m_nCalledCount] = m_nCalledLastNumber;
		} while (checkInCalledNumbers(m_nCalledLastNumber));
		m_nCalledCount++;
		g_number_panel[(m_nCalledLastNumber - 1) / NUMBER_PANEL_COL_COUNT][(m_nCalledLastNumber - 1) % NUMBER_PANEL_COL_COUNT].bCalled = true;
		m_userScreen->setPanelMatchInfo(nPrevLastCalledNumber, 1);
		m_userScreen->setPanelMatchInfo(m_nCalledLastNumber, 2);
		m_userScreen->m_nCalledLastNumber = m_nCalledLastNumber;
		m_userScreen->m_nNumbersCalled = m_nCalledCount;
		m_userScreen->InvalidateRect(m_userScreen->m_LastNumberRect);
		m_userScreen->InvalidateRect(m_userScreen->m_NumbersCalledRect);
		if (m_userScreen->m_bVerified) {
			m_userScreen->m_bVerified = false;
			m_userScreen->InvalidateRect(m_userScreen->m_LogoPanelRect);
		}
		// 		_stprintf_s(m_numPanel.m_szNumber, _T("%d"), m_nCalledLastNumber);
// 		m_numPanel.Invalidate(TRUE);
		InvalidateRect(m_LastCalledNumberRect);
// 		InvalidateRect(m_LastCalledNumberRect);
		InvalidateRect(m_calledCountRect);
		InvalidateRect(g_number_panel[(m_nCalledLastNumber - 1) / NUMBER_PANEL_COL_COUNT][(m_nCalledLastNumber - 1) % NUMBER_PANEL_COL_COUNT].rect);
		InvalidateRect(g_number_panel[(nPrevLastCalledNumber - 1) / NUMBER_PANEL_COL_COUNT][(nPrevLastCalledNumber - 1) % NUMBER_PANEL_COL_COUNT].rect);
		st_recall_info recall_info;
		recall_info.call_id = m_nCalledCount;
		recall_info.called_number = m_nCalledLastNumber;
		recordCallingAction(recall_info);
		// 		m_szLastHouseMatched = _T("");
		m_nVerifiedCnt = 0;
		int cnt = checkPanelInfo(m_nCalledLastNumber, m_nCurrMatchLevel);
		addToVerifiedInfo();
		UpdateData(FALSE);
		if (cnt > 0)
		{
			TCHAR str[10000]; str[0] = 0;
			m_bNumberVerified = false;
			for (int i = 0; i < cnt; i++)
			{
				if (i > 0)
					_stprintf_s(str, _T("%s, %d"), str, g_npMatchedPanels[i]);
				else if (i == 0)
					_stprintf_s(str, _T("%d"), g_npMatchedPanels[i]);
			}
			if (m_nCurrMatchLevel == 1)
				m_szOneLine.Format(_T("%s"), str);
			if (m_nCurrMatchLevel == 2)
				m_szTwoLine.Format(_T("%s"), str);
			else if (m_nCurrMatchLevel == 3)
				m_szFullHouse.Format(_T("%s"), str);
			UpdateData(FALSE);
// 			if (m_nCurrMatchLevel == 4)
// 				SendMessage(WM_STOP, 0, 0);
		}
	}
	else
	{
		int nPrevLastCalledNumber = m_nCalledLastNumber;
		g_nRecallCount--;
		m_nCalledCount++;
		int cnt = g_recall_info.GetCount();
		st_recall_info recall_info = g_recall_info.GetAt(cnt - g_nRecallCount - 1);
		m_nCalledLastNumber = recall_info.called_number;
		g_number_panel[(m_nCalledLastNumber - 1) / NUMBER_PANEL_COL_COUNT][(m_nCalledLastNumber - 1) % NUMBER_PANEL_COL_COUNT].bCalled = true;
		InvalidateRect(g_number_panel[(m_nCalledLastNumber - 1) / NUMBER_PANEL_COL_COUNT][(m_nCalledLastNumber - 1) % NUMBER_PANEL_COL_COUNT].rect);
		InvalidateRect(g_number_panel[(nPrevLastCalledNumber - 1) / NUMBER_PANEL_COL_COUNT][(nPrevLastCalledNumber - 1) % NUMBER_PANEL_COL_COUNT].rect);
		InvalidateRect(m_LastCalledNumberRect);

// 		_stprintf_s(m_numPanel.m_szNumber, _T("%d"), m_nCalledLastNumber);
// 		m_numPanel.Invalidate(TRUE);
		InvalidateRect(m_calledCountRect);
	}
	return 1;
}


void CBingoCallerDlg::OnBnClickedButtonSetRange()
{
	UpdateData(TRUE);
	m_nFromPanel = m_nPanelRangeFrom;
	m_nToPanel = m_nPanelRangeTo;
	if (m_nCalledCount > 0)
	{
		MessageBox(_T("This setting will be applied when start new game."), _T("Notification"), MB_OK);
	}
	else
	{
		resetPanelInfo();
		getPanelRangeInfo();
	}
}


void CBingoCallerDlg::drawYellowBoxes(CDC* dc)
{
	CRect rect;
	GetClientRect(rect);
	CWnd* pCheckWinner = GetDlgItem(IDC_STATIC_CHECK_WINNER);
	CRect rect1;
	pCheckWinner->GetWindowRect(rect1);
	ScreenToClient(rect1);
	COLORREF clr = RGB(255, 196, 37);
	CBrush br1, br2, br3, br4, br5, * br;
	br1.CreateSolidBrush(clr);
	br2.CreateSolidBrush(RGB(61, 226, 159));
	br3.CreateSolidBrush(RGB(255, 255, 255));
	br4.CreateSolidBrush(RGB(0, 0, 0));
	br5.CreateSolidBrush(RGB(255, 215, 0));
	CBrush* oldBrush = dc->SelectObject(&br1);
	dc->Rectangle(rect1);
	dc->SetBkMode(TRANSPARENT);
	LOGFONT lf; CFont font, font1, font2;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 50; lf.lfWeight = FW_BOLD; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("GalanoGrotesque-BlackItalic"));//Galano Grotesque Black Italic
	font.CreateFontIndirect(&lf);
	CFont* oldfont = dc->SelectObject(&font);
	dc->SetTextColor(RGB(0, 0, 0));
	CRect textRect;
	CWnd* pVerifyWinnerStrBoxWnd = GetDlgItem(IDC_STATIC_VERIFY_WINNER_STR_BOX);
	pVerifyWinnerStrBoxWnd->GetWindowRect(textRect);
	ScreenToClient(textRect);
	int h = DrawText(dc->m_hDC, _T("Verify Winner"), -1, textRect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	dc->SelectObject(&oldfont);
	textRect.top = textRect.bottom;
	textRect.bottom = textRect.top + 30;
	lf.lfHeight = 30;
	font1.CreateFontIndirect(&lf);
	dc->SelectObject(&font1);
	DrawText(dc->m_hDC, _T("Enter Panel Number"), -1, textRect, DT_CENTER | DT_SINGLELINE);
	
	CWnd* pNumberRangeWnd = GetDlgItem(IDC_STATIC_RANGE);
	pNumberRangeWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	dc->Rectangle(rect1);
	CRect rect2;
#ifndef TICKET_ONLEY


	CWnd* pSetRangeStrBoxWnd = GetDlgItem(IDC_STATIC_SET_RANGE_STR_BOX);
	pSetRangeStrBoxWnd->GetWindowRect(textRect);
	ScreenToClient(textRect);	dc->SelectObject(&font);
	DrawText(dc->m_hDC, _T("Set Panel Range in Play"), -1, textRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	dc->SelectObject(&font1);
	CWnd* pStartNumberEdit = GetDlgItem(IDC_EDIT_RANGE_FROM);
	pStartNumberEdit->GetWindowRect(rect1);
	ScreenToClient(rect1);
	textRect.left = rect1.left;
	textRect.right = rect1.right;
	textRect.top = rect1.top - 30;
	textRect.bottom = rect1.top;
	DrawText(dc->m_hDC, _T("Start Number"), -1, textRect, DT_CENTER | DT_SINGLELINE);
	CWnd* pEndNumberEdit = GetDlgItem(IDC_EDIT_RANGE_TO);
	pEndNumberEdit->GetWindowRect(rect2);
	ScreenToClient(rect2);
	textRect.left = rect2.left;
	textRect.right = rect2.right;
	textRect.top = rect2.top - 30;
	textRect.bottom = rect2.top;
	DrawText(dc->m_hDC, _T("End Number"), -1, textRect, DT_CENTER | DT_SINGLELINE);
	textRect.left = rect1.right;
	textRect.right = rect2.left;
	textRect.top = rect1.top;
	textRect.bottom = rect1.bottom-5;
	DrawText(dc->m_hDC, _T("to"), -1, textRect, DT_CENTER | DT_SINGLELINE|DT_VCENTER);

#endif // TICKET_ONLEY
#ifdef TICKET_ONLEY
	GetDlgItem(IDC_EDIT_RANGE_FROM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_RANGE_TO)->ShowWindow(SW_HIDE);
#endif
	lf.lfHeight = 25;
	font2.CreateFontIndirect(&lf);
	dc->SelectObject(&font2);
	CWnd* pWinningPatternsWnd = GetDlgItem(IDC_STATIC_WINNING_PATTERNS);
	pWinningPatternsWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
#ifdef TICKET_ONLEY
	rect2.top -= 30;
	rect2.bottom -= 30;
	DrawText(dc->m_hDC, _T("(Full House Already Included)"), -1, rect2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	rect1.bottom = rect2.top;
	dc->SelectObject(&font);
	DrawText(dc->m_hDC, _T("Winning Patterns"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_BOTTOM);
#else
	DrawText(dc->m_hDC, _T("Winning Patterns(Full House Already Included)"), -1, rect2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
#endif // TICKET_ONLEY
	dc->SelectObject(&font2);
	CWnd* pOneLineCheckWnd = GetDlgItem(IDC_STATIC_ONE_LINE_CHECK);
	pOneLineCheckWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
#ifdef TICKET_ONLEY
	rect2.top -= 20;
	rect2.bottom -= 20;
#endif // TICKET_ONLEY
	DrawText(dc->m_hDC, _T("One Line"), -1, rect2, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	CWnd* pTwoLineCheckWnd = GetDlgItem(IDC_STATIC_TWO_LINE_CHECK);
	pTwoLineCheckWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
#ifdef TICKET_ONLEY
	rect2.top -= 20;
	rect2.bottom -= 20;
#endif // TICKET_ONLEY
	DrawText(dc->m_hDC, _T("Two Line"), -1, rect2, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	CWnd* pCallNumberWnd = GetDlgItem(IDC_STATIC_CALL_NUMBER);
	pCallNumberWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	dc->Rectangle(rect1);
	dc->SelectObject(&br3);
	CWnd* pCallNumberStringBoxWnd = GetDlgItem(IDC_STATIC_LAST_CALLED_STRING_BOX);
	pCallNumberStringBoxWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	dc->Rectangle(rect2);
	dc->SelectObject(&font);
	DrawText(dc->m_hDC, _T("LAST NUMBER"), -1, rect2, DT_CENTER | DT_SINGLELINE |DT_VCENTER);

	dc->SelectObject(&br1);
	CWnd* pNumberCalledWnd = GetDlgItem(IDC_STATIC_NUMBERS_CALLED);
	pNumberCalledWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	dc->Rectangle(rect1);
	dc->SelectObject(&br3);
	CWnd* pCallCountStringBoxWnd = GetDlgItem(IDC_STATIC_CALLED_COUNT_STRING_BOX);
	pCallCountStringBoxWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	dc->Rectangle(rect2);
	dc->SelectObject(&font1);
	DrawText(dc->m_hDC, _T("Numbers Called"), -1, rect2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
// 	lf.lfHeight = 280;
// 	font2.CreateFontIndirect(&lf);
// 	dc->SelectObject(&font2);
// 	TCHAR str[10];
// 	_stprintf_s(str, _T("%d"), m_nCalledCount);
// 	rect1.bottom = rect2.top;
// 	dc->SetTextColor(RGB(255, 255, 255));
// 	DrawText(dc->m_hDC, str, -1, rect1, DT_CENTER | DT_SINGLELINE);

	dc->SelectObject(&br1);
	CWnd* pResultWnd = GetDlgItem(IDC_STATIC_RESULT);
	pResultWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	dc->Rectangle(rect1);

	CWnd* pMatchWnd = GetDlgItem(IDC_STATIC_ONE_LINE_MATCH);
	pMatchWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	CRect rect3;
	rect3.left = rect2.left;
	rect3.right = rect2.right;
	rect3.top = rect1.top;
	rect3.bottom = rect3.top + 70;
	dc->SelectObject(&font);
	DrawText(dc->m_hDC, _T("Matched"), -1, rect3, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	dc->SelectObject(&font1);
	rect3.left += 40;
	rect3.right -= 20;
	rect3.top = rect2.top;
	rect3.bottom = rect3.top + 35;
	DrawText(dc->m_hDC, _T("One line match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_CALCRECT);
	DrawText(dc->m_hDC, _T("One line match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	CWnd* pTwoMatchWnd = GetDlgItem(IDC_STATIC_TWO_LINE_MATCH);
	pTwoMatchWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect3.left = rect2.left+40;
	rect3.right = rect2.right-20;
	rect3.top = rect2.top;
	rect3.bottom = rect3.top + 35;
	DrawText(dc->m_hDC, _T("Two line match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_CALCRECT);
	DrawText(dc->m_hDC, _T("Two line match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	CWnd* pFullHouseMatchWnd = GetDlgItem(IDC_STATIC_FULL_HOUSE_MATCH);
	pFullHouseMatchWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect3.left = rect2.left + 40;
	rect3.right = rect2.right - 20;
	rect3.top = rect2.top;
	rect3.bottom = rect3.top + 35;
	DrawText(dc->m_hDC, _T("Full House match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_CALCRECT);
	DrawText(dc->m_hDC, _T("Full House match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK);

	dc->SelectObject(&font);
	CWnd* pMissedWnd = GetDlgItem(IDC_STATIC_ONE_LINE_MISS);
	pMissedWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect3.left = rect2.left;
	rect3.right = rect2.right;
	rect3.top = rect1.top;
	rect3.bottom = rect3.top + 70;
	DrawText(dc->m_hDC, _T("Missed"), -1, rect3, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	dc->SelectObject(&font1);
	rect3.left += 25;
	rect3.right -= 20;
	rect3.top = rect2.top;
	rect3.bottom = rect3.top + 35;
	DrawText(dc->m_hDC, _T("Missed one line match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_CALCRECT);
	DrawText(dc->m_hDC, _T("Missed one line match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	CWnd* pTwoMissedWnd = GetDlgItem(IDC_STATIC_TWO_LINE_MISS);
	pTwoMissedWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect3.left = rect2.left+25;
	rect3.right = rect2.right-20;
	rect3.top = rect2.top;
	rect3.bottom = rect3.top + 35;
	DrawText(dc->m_hDC, _T("Missed two line match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_CALCRECT);
	DrawText(dc->m_hDC, _T("Missed two line match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	CWnd* pFullHouseMissedWnd = GetDlgItem(IDC_STATIC_FULL_HOUSE_MISS);
	pFullHouseMissedWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect3.left = rect2.left + 15;
	rect3.right = rect2.right - 10;
	rect3.top = rect2.top;
	rect3.bottom = rect3.top + 35;
	DrawText(dc->m_hDC, _T("Missed Full House match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_CALCRECT);
	DrawText(dc->m_hDC, _T("Missed Full House match"), -1, rect3, DT_CENTER | DT_VCENTER | DT_WORDBREAK);

	rect2.left = m_nNumPanelRect.left;
	rect2.right = m_nNumPanelRect.right;
	rect2.top = m_nNumPanelRect.top;
	rect2.bottom = m_nNumPanelRect.bottom;
	dc->SelectObject(&br1);
	dc->Rectangle(rect2);
	dc->SelectObject(&br4);
	rect2.left = m_nNumPanelRect.left + NUMBER_PANEL_ITEM_INTERVAL*1.5;
	rect2.right = m_nNumPanelRect.right - NUMBER_PANEL_ITEM_INTERVAL*1.5;
	rect2.top = m_nNumPanelRect.top + NUMBER_PANEL_ITEM_INTERVAL;
	rect2.bottom = m_nNumPanelRect.bottom - NUMBER_PANEL_ITEM_INTERVAL;
	dc->Rectangle(rect2);

	CWnd* pSessionNameWnd = GetDlgItem(IDC_STATIC_SESSION_NAME);
	pSessionNameWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	dc->SelectObject(&br1);
	dc->Rectangle(rect2);
	rect2.left += 10;
	CWnd* pSessionComboWnd = GetDlgItem(IDC_COMBO_SESSION_NAME);
	pSessionComboWnd->GetWindowRect(rect3);
	ScreenToClient(rect3);
	rect2.right = rect3.left - 10;
	DrawText(dc->m_hDC, _T("Session Name:"), -1, rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	CWnd* pGameNumberWnd = GetDlgItem(IDC_STATIC_GAME_NUMBER);
	pGameNumberWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	dc->SelectObject(&br1);
	dc->Rectangle(rect2);
	CWnd* pGameNumberPadWnd = GetDlgItem(IDC_STATIC_GAME_NUMBER_PAD);
	pGameNumberPadWnd->GetWindowRect(rect3);
	rect2.left += 10;
	rect2.right = rect3.left;
	DrawText(dc->m_hDC, _T("Game Number:"), -1, rect2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	ScreenToClient(rect3);
	dc->SelectObject(&br3);
	dc->Rectangle(rect3);
	TCHAR num_str[10];
	_stprintf_s(num_str, _T("%d"), m_nGameNumber);
	DrawText(dc->m_hDC, num_str, -1, rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	CWnd* pPageColorWnd = GetDlgItem(IDC_STATIC_GAME_COLOR);
	pPageColorWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	dc->SelectObject(&br1);
	dc->Rectangle(rect2);
	CWnd* pColorPadWnd = GetDlgItem(IDC_STATIC_COLOR_PAD);
	pColorPadWnd->GetWindowRect(rect3);
	rect2.left += 10;
	rect2.right = rect3.left;
	DrawText(dc->m_hDC, _T("Page Colour:"), -1, rect2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	ScreenToClient(rect3);
	
	CBrush br_color_pad;
	COLORREF color_pad = getRGBFromColorString(m_currColorStr);
	br_color_pad.CreateSolidBrush(color_pad);
	dc->SelectObject(&br_color_pad);
	dc->Rectangle(rect3);
	if (_tcscmp(m_currColorStr, _T("blue")) == 0 || _tcscmp(m_currColorStr, _T("brown")) == 0 || _tcscmp(m_currColorStr, _T("purple")) == 0 || _tcsstr(m_currColorStr, _T("dark")))
		dc->SetTextColor(RGB(255, 255, 255));
	DrawText(dc->m_hDC, m_currColorStr, -1, rect3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	CWnd* pLogoFileWnd = GetDlgItem(IDC_STATIC_CHOOSE_LOGO);
	dc->SelectObject(&br1);
	pLogoFileWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	dc->Rectangle(rect1);
	CWnd* pLogoEditWnd = GetDlgItem(IDC_EDIT_LOGO_FILE);
	pLogoEditWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect1.left += 50;
	rect1.bottom = rect2.top;
	dc->SelectObject(&font);
	dc->SetTextColor(RGB(0, 0, 0));
	DrawText(dc->m_hDC, _T("Choose Logo File Here"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	CWnd* pAdFileWnd = GetDlgItem(IDC_STATIC_CHOOSE_AD);
	dc->SelectObject(&br1);
	pAdFileWnd->GetWindowRect(rect1);
	ScreenToClient(rect1);
	dc->Rectangle(rect1);
	CWnd* pAdEditWnd = GetDlgItem(IDC_EDIT_AD_FILE);
	pAdEditWnd->GetWindowRect(rect2);
	ScreenToClient(rect2);
	rect1.bottom = rect2.top;
	dc->SelectObject(&font1);
	DrawText(dc->m_hDC, _T("Choose Ad Image File and Advertise text Here"), -1, rect1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);


	dc->SelectObject(oldBrush);
	dc->SelectObject(oldfont);
}


void CBingoCallerDlg::addToVerifiedInfo()
{
	m_VerifiedInfo.cnt = g_nMatchedPanelCnt;
	for (int i = 0; i < g_nMatchedPanelCnt; i++)
	{
		m_VerifiedInfo.panel_info[i].panel_id = g_npMatchedPanels[i];
		m_VerifiedInfo.panel_info[i].bVerified = false;
	}
}


void CBingoCallerDlg::checkAsVerified(int panel_id)
{
	for (int i = 0; i < m_VerifiedInfo.cnt; i++)
	{
		if (m_VerifiedInfo.panel_info[i].panel_id == panel_id)
		{
			if (m_VerifiedInfo.panel_info[i].bVerified == false)
			{
				m_VerifiedInfo.panel_info[i].bVerified = true;
				m_nVerifiedCnt++;
				return;
			}
		}
	}
}


void CBingoCallerDlg::moveUnverifiedPanelsToMissedMatch()
{
	st_verified_info* missedPattern;

	if (m_nCurrMatchLevel == 1)
		missedPattern = &m_onelineMissedInfo;
	else if (m_nCurrMatchLevel == 2)
		missedPattern = &m_twolineMissedInfo;
	else
		missedPattern = &m_fullhouseMissedInfo;
	TCHAR strMatched[10000]; strMatched[0] = 0;
	int nMatchedCnt = 0;
	TCHAR strMissed[10000]; strMissed[0] = 0;
	int nMissedCnt = 0;
	for (int i = 0; i < m_VerifiedInfo.cnt; i++)
	{
		if(m_VerifiedInfo.panel_info[i].bVerified==false && checkInMissedPattern(m_VerifiedInfo.panel_info[i].panel_id))
			continue;
		if (m_VerifiedInfo.panel_info[i].bVerified)
		{
			if (nMatchedCnt == 0)
				_stprintf_s(strMatched, _T("%d"), m_VerifiedInfo.panel_info[i].panel_id);
			else
				_stprintf_s(strMatched, _T("%s, %d"), strMatched, m_VerifiedInfo.panel_info[i].panel_id);
			nMatchedCnt++;
		}
		else
		{
			missedPattern->panel_info[missedPattern->cnt++].panel_id = m_VerifiedInfo.panel_info[i].panel_id;
		}
	}
	for (int i = 0; i < missedPattern->cnt; i++)
	{
		if (i == 0)
			_stprintf_s(strMissed, _T("%d"), missedPattern->panel_info[i].panel_id);
		else
			_stprintf_s(strMissed, _T("%s, %d"), strMissed, missedPattern->panel_info[i].panel_id);
	}
	if (m_nCurrMatchLevel == 1)
	{
		m_szOneLine.Format(_T("%s"), strMatched);
		m_szOneLineMiss.Format(_T("%s"), strMissed);
	}
	else if (m_nCurrMatchLevel == 2)
	{
		m_szTwoLine.Format(_T("%s"), strMatched);
		m_szTwoLineMiss.Format(_T("%s"), strMissed);
	}
	else
	{
		m_szFullHouse.Format(_T("%s"), strMatched);
		m_szFullHouseMiss.Format(_T("%s"), strMissed);
	}
	UpdateData(FALSE);
}


bool CBingoCallerDlg::checkInMissedPattern(int panel_id)
{
	st_verified_info* missedPattern;
	if (m_nCurrMatchLevel == 1)
		missedPattern = &m_onelineMissedInfo;
	else if (m_nCurrMatchLevel == 2)
		missedPattern = &m_twolineMissedInfo;
	else
		missedPattern = &m_fullhouseMissedInfo;
	for (int i = 0; i < missedPattern->cnt; i++)
	{
		if (missedPattern->panel_info[i].panel_id == panel_id)
			return true;
	}
	return false;
}


void CBingoCallerDlg::OnBnClickedCheckOneline()
{
// 	UpdateData(TRUE);
	if (m_bStopped==false &&( g_nRecallCount > 0 || m_nCalledCount > 0))
	{
		MessageBox(_T("Can't change setting when calling"), _T("Warning"), MB_OK);
		UpdateData(FALSE);
		return;
	}
	m_bOneLineWinner = !m_bOneLineWinner;
	UpdateData(FALSE);
}


void CBingoCallerDlg::OnBnClickedCheckTwoline()
{
	if (m_bStopped==false && (g_nRecallCount > 0 || m_nCalledCount > 0))
	{
		MessageBox(_T("Can't change setting when calling"), _T("Warning"), MB_OK);
		UpdateData(FALSE);
		return;
	}
	m_bTwoLineWinner = !m_bTwoLineWinner;
	UpdateData(FALSE);
}


void CBingoCallerDlg::copyVerifiedInfo(int currLevel)
{
	CArray<st_panel_verified_info>* new_info;
	if (currLevel == 1)
		new_info = &m_onelineMatchedInfo;
	else if (currLevel == 2)
		new_info = &m_twolineMatchedInfo;
	else
		new_info = &m_fullhouseMatchedInfo;
	new_info->RemoveAll();
	for (int i = 0; i < m_VerifiedInfo.cnt; i++)
	{
		if (m_VerifiedInfo.panel_info[i].bVerified)
		{
			new_info->Add(m_VerifiedInfo.panel_info[i]);
		}
	}
}


void CBingoCallerDlg::makeWinnerString(TCHAR* str)
{
	TCHAR OneLineStr[2000]; OneLineStr[0] = 0;
	if (m_bOneLineWinner)
	{
		for (int i = 0; i < m_onelineMatchedInfo.GetCount(); i++)
		{
			if (i == 0)
				_stprintf_s(OneLineStr, _T("One line winners: %d"), m_onelineMatchedInfo.GetAt(i).panel_id);
			else
				_stprintf_s(OneLineStr, _T("%s, %d"), OneLineStr, m_onelineMatchedInfo.GetAt(i).panel_id);
		}
	}
	TCHAR TwoLineStr[2000]; TwoLineStr[0] = 0;
	if (m_bTwoLineWinner)
	{
		for (int i = 0; i < m_twolineMatchedInfo.GetCount(); i++)
		{
			if (i == 0)
				_stprintf_s(TwoLineStr, _T("Two line winners: %d"), m_twolineMatchedInfo.GetAt(i).panel_id);
			else
				_stprintf_s(TwoLineStr, _T("%s, %d"), TwoLineStr, m_twolineMatchedInfo.GetAt(i).panel_id);
		}
	}
	TCHAR fullHouseStr[2000]; fullHouseStr[0] = 0;
	if (m_nCurrMatchLevel == 3)
	{
		for (int i = 0; i < m_fullhouseMatchedInfo.GetCount(); i++)
		{
			if (i == 0)
				_stprintf_s(fullHouseStr, _T("Full house winners: %d"), m_fullhouseMatchedInfo.GetAt(i).panel_id);
			else
				_stprintf_s(fullHouseStr, _T("%s, %d"), fullHouseStr, m_fullhouseMatchedInfo.GetAt(i).panel_id);
		}
	}
	_stprintf_s(str, 10000, _T("%s\r\n%s\r\n%s"), OneLineStr, TwoLineStr, fullHouseStr);
}


void CBingoCallerDlg::rearrangeVerifiedArrayInfo()
{
	int idx = 0;
	while (m_onelineMatchedInfo.GetCount()>0 && idx < m_onelineMatchedInfo.GetCount())
	{
		bool removed = false;
		for (int i = 0; i < m_twolineMatchedInfo.GetCount(); i++)
		{
			if (m_onelineMatchedInfo.GetAt(idx).panel_id == m_twolineMatchedInfo.GetAt(i).panel_id)
			{
				m_onelineMatchedInfo.RemoveAt(idx, 1);
				removed = true;
				break;
			}
		}
		if(removed)
			continue;
		for (int i = 0; i < m_fullhouseMatchedInfo.GetCount(); i++)
		{
			if (m_onelineMatchedInfo.GetAt(idx).panel_id == m_fullhouseMatchedInfo.GetAt(i).panel_id)
			{
				m_onelineMatchedInfo.RemoveAt(idx, 1);
				removed = true;
				break;
			}
		}
		if(removed)
			continue;
		idx++;
	}
	idx = 0;
	while (m_twolineMatchedInfo.GetCount() > 0 && idx < m_twolineMatchedInfo.GetCount())
	{
		bool removed = false;
		for (int i = 0; i < m_fullhouseMatchedInfo.GetCount(); i++)
		{
			if (m_twolineMatchedInfo.GetAt(idx).panel_id == m_fullhouseMatchedInfo.GetAt(i).panel_id)
			{
				m_twolineMatchedInfo.RemoveAt(idx, 1);
				removed = true;
				break;
			}
		}
		if (removed)
			continue;
		idx++;
	}
}


void initializeServer()
{
	WORD		wVersionRequested;
	WSADATA		wsaData;
	SOCKADDR_IN servAddr, cliAddr; //Socket address information
	int			err;
	int			bytesSent;
	char        buf[50];

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0) {
		MessageBox(g_pCallerDlg->m_hWnd, _T("WSAStartup error"), _T("Error"), MB_OK);
		WSACleanup();
		return;
	}

	servAddr.sin_family = AF_INET; // address family Internet
	servAddr.sin_port = htons(SERVER_PORT); //Port to connect on
	servAddr.sin_addr.s_addr = inet_addr(IPAddress); //Target IP


	g_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (g_ServerSocket == INVALID_SOCKET)
	{
		MessageBox(g_pCallerDlg->m_hWnd, _T("Socket error"), _T("Error"), MB_OK);
		WSACleanup();
		return; //Couldn't create the socket
	}

	int x = bind(g_ServerSocket, reinterpret_cast<SOCKADDR*>(&servAddr), sizeof(servAddr));
	if (x == SOCKET_ERROR)
	{
		MessageBox(g_pCallerDlg->m_hWnd, _T("Binding failed."), _T("Error"), MB_OK);
		WSACleanup();
		return; //Couldn't connect
	}

	listen(g_ServerSocket, 5);
	int xx = sizeof(cliAddr);
	g_ClientSocket = accept(g_ServerSocket, reinterpret_cast<SOCKADDR*>(&cliAddr), &xx);
	TCHAR str[100];
	_stprintf_s(str, _T("Connection established. New socket num is %d"), g_ClientSocket);
	MessageBox(g_pCallerDlg->m_hWnd, str, _T("Notification"), MB_OK);
}

UINT connectToClient()
{
	initializeServer();
	char buf[100];
	while (true)
	{
// 		n = recv(s2, buf, 50, 0);
// 		if (n <= 0) { cout << "Got nothing" << endl; break; }
// 		buf[n] = 0;
// 		if (!strcmp(buf, "HELLO"))
// 		{// Initial communication
// 			iRand = makeRand();
// 			auto sRand = std::to_string(iRand);
// 			cout << "Sending random number " << iRand << " to the client." << endl;
// 			bytesSent = send(s2, sRand.c_str(), sRand.length(), 0);
// 
// 			continue;
// 		}
// 		std::string sNum(buf);
// 		try
// 		{
// 			iRand = stoi(sNum);
// 			cout << "Server got " << "\"" << iRand << "\"" << endl;
// 			std::this_thread::sleep_for(std::chrono::seconds(1));
// 			cout << "Sending \"" << ++iRand << "\"" << " to client" << endl;
// 			auto sRand = std::to_string(iRand);
// 			bytesSent = send(s2, sRand.c_str(), sRand.length(), 0);
// 		}
// 		catch (const std::invalid_argument& ex)
// 		{
// 			std::cerr << "Invalid argument while converting string to number" << endl;
// 			std::cerr << "Error: " << ex.what() << endl;
// 			break;
// 		}
// 		catch (const std::out_of_range& ex)
// 		{
// 			std::cerr << "Invalid argument while converting string to number" << endl;
// 			std::cerr << "Error: " << ex.what() << endl;
// 			break;
// 		}
	}

}


void CBingoCallerDlg::setColorInfo()
{
	_tcscpy_s(color_info[0].color_name, _T("gold"));
	color_info[0].color = RGB(255, 215, 0);
	_tcscpy_s(color_info[1].color_name, _T("purple"));
	color_info[1].color = RGB(128, 0, 128);
	_tcscpy_s(color_info[2].color_name, _T("yellow"));
	color_info[2].color = RGB(255, 255, 0);
	_tcscpy_s(color_info[3].color_name, _T("pink"));
	color_info[3].color = RGB(255, 192, 203);
	_tcscpy_s(color_info[4].color_name, _T("green"));
	color_info[4].color = RGB(0, 255, 0);
	_tcscpy_s(color_info[5].color_name, _T("orange"));
	color_info[5].color = RGB(255, 166, 0);
	_tcscpy_s(color_info[6].color_name, _T("blue"));
	color_info[6].color = RGB(0, 0, 255);
	_tcscpy_s(color_info[7].color_name, _T("red"));
	color_info[7].color = RGB(255, 0, 0);
	_tcscpy_s(color_info[8].color_name, _T("grey"));
	color_info[8].color = RGB(128, 128, 128);
	_tcscpy_s(color_info[9].color_name, _T("brown"));
	color_info[9].color = RGB(165, 42, 42);
	_tcscpy_s(color_info[10].color_name, _T("white"));
	color_info[10].color = RGB(255, 255, 255);
	_tcscpy_s(color_info[11].color_name, _T("lightpurple"));
	color_info[11].color = RGB(160, 32, 240);
	_tcscpy_s(color_info[12].color_name, _T("lightyellow"));
	color_info[12].color = RGB(255, 255, 224);
	_tcscpy_s(color_info[13].color_name, _T("lightpink"));
	color_info[13].color = RGB(255, 182, 193);
	_tcscpy_s(color_info[14].color_name, _T("lightgreen"));
	color_info[14].color = RGB(144, 238, 144);
	_tcscpy_s(color_info[15].color_name, _T("lightorange"));
	color_info[15].color = RGB(255, 207, 158);
	_tcscpy_s(color_info[16].color_name, _T("lightblue"));
	color_info[16].color = RGB(173, 216, 230);
	_tcscpy_s(color_info[17].color_name, _T("lightred"));
	color_info[17].color = RGB(255, 204, 203);
	_tcscpy_s(color_info[18].color_name, _T("lightgrey"));
	color_info[18].color = RGB(211, 211, 211);
	_tcscpy_s(color_info[19].color_name, _T("lightbrown"));
	color_info[19].color = RGB(181, 101, 29);
	_tcscpy_s(color_info[20].color_name, _T("silver"));
	color_info[20].color = RGB(192, 192, 192);
	_tcscpy_s(color_info[21].color_name, _T("darkpurple"));
	color_info[21].color = RGB(128, 0, 191);
	_tcscpy_s(color_info[22].color_name, _T("darkyellow"));
	color_info[22].color = RGB(102, 102, 0);
	_tcscpy_s(color_info[23].color_name, _T("darkpink"));
	color_info[23].color = RGB(231, 84, 128);
	_tcscpy_s(color_info[24].color_name, _T("darkgreen"));
	color_info[24].color = RGB(0, 100, 0);
	_tcscpy_s(color_info[25].color_name, _T("darkorange"));
	color_info[25].color = RGB(255, 140, 0);
	_tcscpy_s(color_info[26].color_name, _T("darkblue"));
	color_info[26].color = RGB(0, 0, 139);
	_tcscpy_s(color_info[27].color_name, _T("darkred"));
	color_info[27].color = RGB(139, 0, 0);
	_tcscpy_s(color_info[28].color_name, _T("darkgrey"));
	color_info[28].color = RGB(169, 169, 169);
	_tcscpy_s(color_info[29].color_name, _T("darkbrown"));
	color_info[29].color = RGB(101, 67, 33);
	_tcscpy_s(m_currColorStr, _T("gold"));
}


COLORREF CBingoCallerDlg::getRGBFromColorString(TCHAR *color_str)
{
	for (int i = 0; i < 30; i++)
	{
		if (_tcscmp(color_info[i].color_name, color_str) == 0)
			return color_info[i].color;
	}
	return RGB(255, 215, 0);
}


void CBingoCallerDlg::loadPanelInfo()
{
	UpdateData(TRUE);
#ifndef TICKET_ONLEY
	if (m_nPanelRangeFrom > 0 && m_nPanelRangeTo > m_nPanelRangeFrom)
		loadPanelInfoForRange();
	else
#endif
		loadPanelInfoForTicket();
}


void CBingoCallerDlg::createUserScreen()
{
	m_userScreen = new CUserScreen(this);
	if (m_userScreen)
	{
		BOOL ret = m_userScreen->Create(IDD_DIALOG_USER_SCREEN, this);
		if (!ret)
		{
			AfxMessageBox(_T("Error creating user screen"));
		}
		m_userScreen->ShowWindow(SW_SHOW);
	}
}


void CBingoCallerDlg::OnBnClickedButtonLogo()
{
	TCHAR szFilters[] = _T("Image Files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp||");//|All Files (*.*)|*.*||

// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, _T("png"), NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		m_szLogoFilename = fileDlg.GetPathName();
		_tcscpy_s(m_userScreen->m_szLogoFile, (LPCTSTR)m_szLogoFilename);
		m_userScreen->InvalidateRect(m_userScreen->m_LogoPanelRect);
		UpdateData(FALSE);
	}

}


void CBingoCallerDlg::OnBnClickedButtonAdvertise()
{
	TCHAR szFilters[] = _T("Image Files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp||");//|All Files (*.*)|*.*||

// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, _T("png"), NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		m_szAdFilename = fileDlg.GetPathName();
		_tcscpy_s(m_userScreen->m_szAdFile, (LPCTSTR)m_szAdFilename);
		m_userScreen->InvalidateRect(m_userScreen->m_advertiseRect);
		UpdateData(FALSE);
	}
}


void CBingoCallerDlg::OnBnClickedButtonAdTextSet()
{
	UpdateData(TRUE);
	_tcscpy_s(m_userScreen->m_szAdText, (LPCTSTR)m_szAdText);
	m_userScreen->InvalidateRect(m_userScreen->m_advertiseRect);
}


void CBingoCallerDlg::resetUserScreen()
{
	m_userScreen->resetNumberArray();
	m_userScreen->InvalidateRect(m_userScreen->m_nNumPanelRect);
	m_userScreen->resetVerifiedArray();
	m_userScreen->InvalidateRect(m_userScreen->m_LogoPanelRect);
	m_userScreen->m_nNumbersCalled = 0;
	m_userScreen->m_nCalledLastNumber = 0;
	m_userScreen->InvalidateRect(m_userScreen->m_LastNumberRect);
	m_userScreen->InvalidateRect(m_userScreen->m_NumbersCalledRect);
	m_userScreen->m_bVerified = false;
	m_userScreen->m_nGameNumber = m_nGameNumber;
	m_userScreen->InvalidateRect(m_userScreen->m_GameNumberRect);
	m_userScreen->m_nGameLevel = 1;
}


void CBingoCallerDlg::loadPanelInfoForRange()
{
	char query[300];
	g_nPanelInfoCnt = 0;
	
	for(int panel_id = m_nPanelRangeFrom; panel_id < m_nPanelRangeTo; panel_id++)
	{
		int table_id = panel_id % 6;
		if (table_id == 0)
			table_id = 6;
		sprintf_s(query, "select * from panel_%d where panel_id='%d'", table_id, panel_id);
		if (mysql_query(mysql, query) == 0)
		{
			MYSQL_RES* res1 = mysql_store_result(mysql);
			if (res1->row_count > 0)
			{
				MYSQL_ROW row1 = mysql_fetch_row(res1);
				g_panel_info[g_nPanelInfoCnt].panel_id = atoi(row1[1]);
				char* str = row1[2];
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 9; k++)
					{
						char ch[3];
						strncpy_s(ch, str, 2);
						ch[2] = 0;
						str += 2;
						g_panel_info[g_nPanelInfoCnt].items[k][j].val = atoi(ch);
						g_panel_info[g_nPanelInfoCnt].items[k][j].bmatch = false;
					}
				}
				g_nPanelInfoCnt++;
			}
			mysql_free_result(res1);
		}
	}
}

void CBingoCallerDlg::getCustomerName(int session_id, int panel_id, TCHAR* customerName)
{
	char query[200];
	sprintf_s(query, "select customer_name from soldtickets_new where session_id='%d' and panel_id='%d'", session_id, panel_id);
	if (mysql_query(mysql, query) == 0) {
		MYSQL_RES* res = mysql_store_result(mysql);
		if (res->row_count > 0) {
			MYSQL_ROW row = mysql_fetch_row(res);
			CharToTCHAR(row[0], customerName);
		}
		else {
			customerName[0] = 0;
		}
		mysql_free_result(res);
	}
}
void CBingoCallerDlg::loadPanelInfoForTicket()
{
	COLORREF color = getRGBFromColorString(m_currColorStr);
	_tcscpy_s(m_userScreen->m_szColor, m_currColorStr);
	m_userScreen->m_color = color;
	m_userScreen->InvalidateRect(m_userScreen->m_colorRect);
	char query[300];
	g_nPanelInfoCnt = 0;
	char c_szColor[20];
	wcstombs(c_szColor, m_currColorStr, wcslen(m_currColorStr) + 1);
	CTime time = CTime::GetCurrentTime();
	char szDateTime[50];
	sprintf_s(szDateTime, "%04d-%02d-%02d %02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
	char szDateDefaultTime[50];
	sprintf_s(szDateDefaultTime, "%04d-%02d-%02d 00:00", time.GetYear(), time.GetMonth(), time.GetDay());
	sprintf_s(query, "select panel_id from soldtickets_new where session_id='%d' and color='%s' and sold='1'", m_nCurrSessionId, c_szColor);
// 	sprintf_s(query, "select panel_id from soldtickets where (date_from<='%s' and date_to>='%s' and color='%s') or (date_from='%s' and date_to='%s' and color='%s')", szDateTime, szDateTime, c_szColor, szDateDefaultTime, szDateDefaultTime, c_szColor);
	if (mysql_query(mysql, query) == 0)
	{
		res = mysql_store_result(mysql);
		while (row = mysql_fetch_row(res))
		{
			int panel_id = atoi(row[0]);
			int table_id = panel_id % 6;
			if (table_id == 0)
				table_id = 6;
			sprintf_s(query, "select * from panel_%d where panel_id='%d'", table_id, panel_id);
			if (mysql_query(mysql, query) == 0)
			{
				MYSQL_RES* res1 = mysql_store_result(mysql);
				if (res1->row_count > 0)
				{
					MYSQL_ROW row1 = mysql_fetch_row(res1);
					g_panel_info[g_nPanelInfoCnt].panel_id = atoi(row1[1]);
					char* str = row1[2];
					for (int j = 0; j < 3; j++)
					{
						for (int k = 0; k < 9; k++)
						{
							char ch[3];
							strncpy_s(ch, str, 2);
							ch[2] = 0;
							str += 2;
							g_panel_info[g_nPanelInfoCnt].items[k][j].val = atoi(ch);
							g_panel_info[g_nPanelInfoCnt].items[k][j].bmatch = false;
						}
					}
					g_nPanelInfoCnt++;
				}
				mysql_free_result(res1);
			}
		}
		mysql_free_result(res);
	}
}


void CBingoCallerDlg::OnEnChangeEditRangeFrom()
{
// 	UpdateData(FALSE);
}


void CBingoCallerDlg::OnEnChangeEditRangeTo()
{
// 	UpdateData(FALSE);
}


void CBingoCallerDlg::OnBnClickedButtonAdImageRemove()
{
	m_szAdFilename = _T("");
	_tcscpy_s(m_userScreen->m_szAdFile, (LPCTSTR)m_szAdFilename);
	m_userScreen->InvalidateRect(m_userScreen->m_advertiseRect);
	UpdateData(FALSE);
}


void CBingoCallerDlg::loadSessionNames()
{
	char query[200];
	m_nSessionCnt = 0;
	m_nCurrSessionId = 0;
	CTime time = CTime::GetCurrentTime();
	char szDateTime[50];
	sprintf_s(szDateTime, "%04d-%02d-%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	sprintf_s(query, "select *from game_session_info where date >= '%s'", szDateTime);
	if (mysql_query(mysql, query) == 0) {
		MYSQL_RES* res = mysql_store_result(mysql);
		while (MYSQL_ROW row = mysql_fetch_row(res))
		{
			m_npSession_ids[m_nSessionCnt++] = atoi(row[0]);
			TCHAR temp[100];
			CharToTCHAR(row[1], temp);
			m_comboSession.AddString(temp);
		}
		if (m_nSessionCnt > 0) {
			m_comboSession.SetCurSel(0);
			m_nCurrSessionId = m_npSession_ids[0];
			loadPanelInfo();
		}
		mysql_free_result(res);
	}
}


void CBingoCallerDlg::OnCbnSelchangeComboSessionName()
{
	int idx = m_comboSession.GetCurSel();
	if (idx == -1)
	{
		m_nCurrSessionId = 0;
		return;
	}
	m_nCurrSessionId = m_npSession_ids[idx];
 	loadPanelInfo();
}
