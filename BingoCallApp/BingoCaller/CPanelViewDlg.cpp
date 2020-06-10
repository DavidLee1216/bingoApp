// CPanelViewDlg.cpp : implementation file
//

#include "pch.h"
#include "BingoCaller.h"
#include "CPanelViewDlg.h"
#include "afxdialogex.h"
#include "BingoCallerDlg.h"

// CPanelViewDlg dialog
// #define CARD_SHOW

int g_nViewNumberPanelMargin = 80;
int g_nViewNumberPanelTopMargin = 50;
// int g_nViewNumberPanelTopMargin = 30;

int VIEW_NUMBER_PANEL_ITEM_WIDTH = 40;
int VIEW_NUMBER_PANEL_ITEM_HEIGHT = 30;
int VIEW_NUMBER_PANEL_ITEM_INTERVAL = 2;

int g_nViewPanelInterval = 10;

IMPLEMENT_DYNAMIC(CPanelViewDlg, CDialogEx)

CPanelViewDlg::CPanelViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PANEL, pParent)
{
	m_pParentWnd = (CDialog*)pParent;
	m_pPrintDC = 0;
	m_nPanelId = 0;
	m_pPD = 0;
}

CPanelViewDlg::~CPanelViewDlg()
{
	int kk = 0;
}

void CPanelViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPanelViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PRINT, &CPanelViewDlg::OnBnClickedPrint)
	ON_COMMAND(IDCLOSE, &CPanelViewDlg::OnIdclose)
END_MESSAGE_MAP()


struct st_number_panel
{
	CRect rect;
	int number;
	bool bCalled;
	bool bWinned;
	st_number_panel() {
		bCalled = false;
		bWinned = false;
	}
};

st_number_panel g_st_numberPanel[9][18];

extern int g_nNumberPanelMargin;
extern int g_nNumberPanelTopMargin;

extern int NUMBER_PANEL_ITEM_WIDTH;
extern int NUMBER_PANEL_ITEM_HEIGHT;
extern int NUMBER_PANEL_ITEM_INTERVAL;

int g_nWinnedLines[3];
int CPanelViewDlg::getWinnedLines(int panel_id)
{
	int nMatchedLine = 0;
	for (int j = 0; j < 3; j++)
	{
		int nMatchedCnt = 0;
		for (int i = 0; i < 9; i++)
		{
			if (((CBingoCallerDlg*)m_pParentWnd)->checkInCalledNumbers(((CBingoCallerDlg*)m_pParentWnd)->m_nppWinnerPanelInfo[panel_id][i][j]))
				nMatchedCnt++;
		}
		if (nMatchedCnt == 5)
		{
			g_nWinnedLines[nMatchedLine++] = j;
		}
	}
	return nMatchedLine;
}
BOOL CPanelViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int nTableId = m_nPanelId % 6;
	if (nTableId == 0)
		nTableId = 6;
	int matchedLineCnt = getWinnedLines(nTableId-1);
	for (int panel_id = 0; panel_id < 6; panel_id++)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int row_id = panel_id * 3 + j;
				g_st_numberPanel[i][row_id].rect.left = g_nViewNumberPanelMargin + i * (VIEW_NUMBER_PANEL_ITEM_WIDTH + VIEW_NUMBER_PANEL_ITEM_INTERVAL);
				g_st_numberPanel[i][row_id].rect.right = g_st_numberPanel[i][row_id].rect.left + VIEW_NUMBER_PANEL_ITEM_WIDTH;
#ifdef CARD_SHOW
				g_st_numberPanel[i][row_id].rect.top = g_nViewNumberPanelTopMargin + g_nViewPanelInterval * panel_id + row_id * VIEW_NUMBER_PANEL_ITEM_HEIGHT + row_id * VIEW_NUMBER_PANEL_ITEM_INTERVAL;
#else
				g_st_numberPanel[i][row_id].rect.top = g_nViewNumberPanelTopMargin + g_nViewPanelInterval*0 + j * VIEW_NUMBER_PANEL_ITEM_HEIGHT + j * VIEW_NUMBER_PANEL_ITEM_INTERVAL;
#endif
				g_st_numberPanel[i][row_id].rect.bottom = g_st_numberPanel[i][row_id].rect.top + VIEW_NUMBER_PANEL_ITEM_HEIGHT;
				g_st_numberPanel[i][row_id].number = ((CBingoCallerDlg*)m_pParentWnd)->m_nppWinnerPanelInfo[panel_id][i][j];
				if (((CBingoCallerDlg*)m_pParentWnd)->checkInCalledNumbers(((CBingoCallerDlg*)m_pParentWnd)->m_nppWinnerPanelInfo[panel_id][i][j]))
					g_st_numberPanel[i][row_id].bCalled = true;
				else
					g_st_numberPanel[i][row_id].bCalled = false;
				g_st_numberPanel[i][row_id].bWinned = false;
			}
		}
	}
	if (matchedLineCnt > 0)
	{
		for (int i = 0; i < matchedLineCnt; i++)
		{
			int row_id = (nTableId-1)*3+g_nWinnedLines[i];
			for (int j = 0; j < 9; j++)
			{
				if (g_st_numberPanel[j][row_id].bCalled == true)
					g_st_numberPanel[j][row_id].bWinned = true;
				else
					g_st_numberPanel[j][row_id].bWinned = false;
			}
		}
	}

#ifdef CARD_SHOW
	CRect rct, rect1;
	GetClientRect(rct);
	MoveWindow(400, 100, rct.Width(), rct.Height() * 3);
	CWnd *printWnd = GetDlgItem(IDC_PRINT);
	printWnd->GetClientRect(rect1);
	printWnd->MoveWindow(100, rct.Height()*3 - 100, rect1.Width(), rect1.Height());
	CWnd* okWnd = GetDlgItem(IDOK);
	okWnd->GetClientRect(rect1);
	okWnd->MoveWindow(rct.Width()/2+100, rct.Height()*3 - 100, rect1.Width(), rect1.Height());
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPanelViewDlg::OnPaint()
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


void CPanelViewDlg::OnDraw(CDC* dc)
{
	LOGFONT lf, lf1; CFont font, font1;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 40; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("Arial"));
	font.CreateFontIndirect(&lf);
	memset(&lf1, 0, sizeof(LOGFONT));
	lf1.lfHeight = 30; lf1.lfWidth = 0;
	_tcscpy_s(lf1.lfFaceName, _T("Arial"));
	font1.CreateFontIndirect(&lf1);
	CFont* oldfont = dc->SelectObject(&font);
	SetBkMode(dc->m_hDC, TRANSPARENT);
	COLORREF clr = RGB(230, 230, 230);
	CBrush br1, br2, br3, br4, * br;
	br1.CreateSolidBrush(clr);
	br2.CreateSolidBrush(RGB(0, 50, 255));
	br3.CreateSolidBrush(RGB(255, 155, 0));
	br4.CreateSolidBrush(RGB(255, 0, 0));
	TCHAR str_num[30];
	CBrush* oldBrush = dc->SelectObject(&br1);
	int table_id = m_nPanelId % 6 - 1;
	if (table_id == -1)
		table_id = 5;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			if (g_st_numberPanel[i][j].bCalled && g_st_numberPanel[i][j].number == ((CBingoCallerDlg*)m_pParentWnd)->m_nCalledLastNumber)
				br = &br3;
			else if (g_st_numberPanel[i][j].bWinned)
				br = &br4;
			else if (g_st_numberPanel[i][j].bCalled)
				br = &br2;
			else
				br = &br1;
			dc->SelectObject(br);
			int panel_id = j / 3;
#ifndef CARD_SHOW
			if(table_id==panel_id)  //only show winner table
#endif
				dc->Rectangle(g_st_numberPanel[i][j].rect);
			if(g_st_numberPanel[i][j].number > 0)
				_stprintf_s(str_num, _T("%d"), g_st_numberPanel[i][j].number);
			else if(g_st_numberPanel[i][j].number==0)
				_stprintf_s(str_num, _T(""));
			dc->SelectObject(font);
			dc->SetTextColor(RGB(0, 0, 0));
#ifndef CARD_SHOW
			if (table_id == panel_id) //only show winner table
#endif
				DrawText(dc->m_hDC, str_num, -1, g_st_numberPanel[i][j].rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (i == 0 && j % 3==0)
			{
				dc->SelectObject(&font1);
				CRect rect;
				rect.left = g_nViewNumberPanelMargin + 9 * (VIEW_NUMBER_PANEL_ITEM_WIDTH + VIEW_NUMBER_PANEL_ITEM_INTERVAL) + 30;
				rect.right = rect.left + 200;
#ifndef CARD_SHOW
				rect.top = g_nViewNumberPanelTopMargin;
#else
				rect.top = g_nViewNumberPanelTopMargin + g_nViewPanelInterval * panel_id + (panel_id * 3) * (VIEW_NUMBER_PANEL_ITEM_HEIGHT + VIEW_NUMBER_PANEL_ITEM_INTERVAL);
#endif
				rect.bottom = rect.top + VIEW_NUMBER_PANEL_ITEM_HEIGHT;
				_stprintf_s(str_num, _T("%d"), ((CBingoCallerDlg*)m_pParentWnd)->m_npWinnerCardSerialInfo[panel_id]);
				if (((CBingoCallerDlg*)m_pParentWnd)->m_npWinnerCardSerialInfo[panel_id] == m_nPanelId)
					dc->SetTextColor(RGB(255, 0, 0));
				else
					dc->SetTextColor(RGB(0, 0, 0));

#ifndef CARD_SHOW
				if (table_id == panel_id) //only show winner table
#endif
					DrawText(dc->m_hDC, str_num, -1, rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}
	dc->SelectObject(&oldfont);
	dc->SelectObject(oldBrush);
}


void CPanelViewDlg::resetPanelCallState()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			g_st_numberPanel[i][j].bCalled = false;
		}
	}
}


void CPanelViewDlg::OnBnClickedPrint()
{
	OnOK();
//  	print();
}

void CPanelViewDlg::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

// 	CView::OnEndPrinting(pDC, pInfo);
}


void CPanelViewDlg::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	// TODO: Add your specialized code here and/or call the base class

// 	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}


void CPanelViewDlg::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

// 	CView::OnPrepareDC(pDC, pInfo);
}


void CPanelViewDlg::OnPreparePrinting(CPrintInfo* pInfo)
{

	// TODO:  call DoPreparePrinting to invoke the Print dialog box
// 	CDC dc;
// 	dc.Attach(pInfo->m_pPD->GetPrinterDC());
// 	int horz = dc.GetDeviceCaps(HORZRES);
// 	int vert = dc.GetDeviceCaps(VERTRES);
// 
// 	CRect rect;
// 	GetClientRect(rect);
// 
// 	pInfo->m_rectDraw.left = pInfo->m_rectDraw.left * rect.Width() / horz;
	pInfo->m_rectDraw.left += 100;
	pInfo->m_rectDraw.right -= 100;
	pInfo->m_rectDraw.top += 100;
	pInfo->m_rectDraw.bottom -= 100;
	pInfo->m_rectDraw.NormalizeRect();
	m_pPrintDC->DPtoLP(&pInfo->m_rectDraw);
// 	return CView::OnPreparePrinting(pInfo);
}


void CPanelViewDlg::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	OnPrintDraw(pDC, pInfo);

// 	CView::OnPrint(pDC, pInfo);
}


void CPanelViewDlg::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

// 	CView::OnBeginPrinting(pDC, pInfo);
}

CDC* CPanelViewDlg::CreatePrintDC()
{
	if (m_pPD)
	{
		CDC* pDC = new CDC();
		pDC->Attach(m_pPD->hDC);
		return pDC;
	}

	return NULL;
}

void CPanelViewDlg::OnPrintDraw(CDC* dc, CPrintInfo* pInfo)
{
	CRect rect;
	GetClientRect(rect);
	int horz = dc->GetDeviceCaps(HORZRES);
	int vert = dc->GetDeviceCaps(VERTRES);
	int horzsize = dc->GetDeviceCaps(HORZSIZE);
	int vertsize = dc->GetDeviceCaps(VERTSIZE);

	LPtoDP(dc->m_hDC, (LPPOINT)&rect, 2);
	dc->SetBkMode(TRANSPARENT);
	COLORREF Clr = RGB(255, 255, 255);
	CBrush brush(Clr);
	dc->FillRect(rect, &brush);
// 	DPtoLP(dc->m_hDC, (LPPOINT)&rect, 2);

	LOGFONT lf, lf1; CFont font, font1;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 40; lf.lfWidth = 0;
	_tcscpy_s(lf.lfFaceName, _T("Arial"));
	font.CreateFontIndirect(&lf);
	memset(&lf1, 0, sizeof(LOGFONT));
	lf1.lfHeight = 30; lf1.lfWidth = 0;
	_tcscpy_s(lf1.lfFaceName, _T("Arial"));
	font1.CreateFontIndirect(&lf1);
	CFont* oldfont = dc->SelectObject(&font);
	SetBkMode(dc->m_hDC, TRANSPARENT);
	COLORREF clr = RGB(230, 230, 230);
	CBrush br1, br2, br3, br4, * br;
	br1.CreateSolidBrush(clr);
	br2.CreateSolidBrush(RGB(0, 50, 255));
	br3.CreateSolidBrush(RGB(255, 155, 0));
	br4.CreateSolidBrush(RGB(255, 0, 0));
	TCHAR str_num[30];
	CBrush* oldBrush = dc->SelectObject(&br1);
	int table_id = m_nPanelId % 6 - 1;
	if (table_id == -1)
		table_id = 5;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			if (g_st_numberPanel[i][j].bCalled && g_st_numberPanel[i][j].number == ((CBingoCallerDlg*)m_pParentWnd)->m_nCalledLastNumber)
				br = &br3;
			else if (g_st_numberPanel[i][j].bWinned)
				br = &br4;
			else if (g_st_numberPanel[i][j].bCalled)
				br = &br2;
			else
				br = &br1;
			dc->SelectObject(br);
			CRect rect;
			rect = g_st_numberPanel[i][j].rect;
			LPtoDP(dc->m_hDC, (LPPOINT)&rect, 2);
			int panel_id = j / 3;
			if (table_id == panel_id)  //only show winner table
				dc->Rectangle(rect);
			if (g_st_numberPanel[i][j].number > 0)
				_stprintf_s(str_num, _T("%d"), g_st_numberPanel[i][j].number);
			else if (g_st_numberPanel[i][j].number == 0)
				_stprintf_s(str_num, _T(""));
			dc->SelectObject(font);
			dc->SetTextColor(RGB(0, 0, 0));
			if (table_id == panel_id)  //only show winner table
				DrawText(dc->m_hDC, str_num, -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (i == 0 && j % 3 == 0)
			{
				dc->SelectObject(&font1);
// 				CRect rect;
				rect.left = g_nViewNumberPanelMargin + 9 * (VIEW_NUMBER_PANEL_ITEM_WIDTH + VIEW_NUMBER_PANEL_ITEM_INTERVAL) + 30;
				rect.right = rect.left + 200;
				rect.top = g_nViewNumberPanelTopMargin;
// 				rect.top = g_nViewNumberPanelTopMargin + g_nViewPanelInterval * panel_id + (panel_id * 3) * (VIEW_NUMBER_PANEL_ITEM_HEIGHT + VIEW_NUMBER_PANEL_ITEM_INTERVAL);
				rect.bottom = rect.top + VIEW_NUMBER_PANEL_ITEM_HEIGHT;
				_stprintf_s(str_num, _T("%d"), ((CBingoCallerDlg*)m_pParentWnd)->m_npWinnerCardSerialInfo[panel_id]);
				if (((CBingoCallerDlg*)m_pParentWnd)->m_npWinnerCardSerialInfo[panel_id] == m_nPanelId)
					dc->SetTextColor(RGB(255, 0, 0));
				else
					dc->SetTextColor(RGB(0, 0, 0));
				LPtoDP(dc->m_hDC, (LPPOINT)&rect, 2);
				if (table_id == panel_id)  //only show winner table
					DrawText(dc->m_hDC, str_num, -1, rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}
	dc->SelectObject(&oldfont);
	dc->SelectObject(oldBrush);

}


void CPanelViewDlg::print()
{
	CPrintDialog printDlg(FALSE);
	if (printDlg.DoModal() == IDCANCEL) return;

	m_pPD = &printDlg.m_pd;

	if (m_pPrintDC == 0)
		m_pPrintDC = CreatePrintDC();
	m_pPrintDC->m_bPrinting = TRUE;
	m_pPrintDC->SetMapMode(MM_LOMETRIC);
	// 	m_pPrintDC->SetMapMode(MM_HIMETRIC);
	// 	m_pPrintDC->SetMapMode(MM_TWIPS);

	CString strTitle;
	strTitle.LoadString(AFX_IDS_APP_TITLE);
	DOCINFO di;
	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = strTitle;

	BOOL bPrintingOK = m_pPrintDC->StartDoc(&di);

	CPrintInfo Info;
	Info.m_rectDraw.SetRect(0, 0, m_pPrintDC->GetDeviceCaps(HORZRES),
		m_pPrintDC->GetDeviceCaps(VERTRES));
	// 	dc.DPtoLP(&Info.m_rectDraw);
	OnPreparePrinting(&Info);
	int nPage = 1;
	Info.SetMaxPage(nPage);

	OnBeginPrinting(m_pPrintDC, &Info);

	for (UINT page = Info.GetMinPage();
		page <= Info.GetMaxPage() && bPrintingOK; page++)
	{
		m_pPrintDC->StartPage();
		Info.m_nCurPage = page;

		OnPrint(m_pPrintDC, &Info);

		bPrintingOK = (m_pPrintDC->EndPage() > 0);
	}

	OnEndPrinting(m_pPrintDC, &Info);

	if (bPrintingOK) m_pPrintDC->EndDoc();
	else m_pPrintDC->AbortDoc();

	m_pPrintDC->Detach();
}



void CPanelViewDlg::OnIdclose()
{
	OnClose();
}
