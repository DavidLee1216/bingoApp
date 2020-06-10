
// PanelDataCreatorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PanelDataCreator.h"
#include "PanelDataCreatorDlg.h"
#include "afxdialogex.h"
#include <mysql.h>
#include <WinSock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//#pragma comment(lib,"libmysqld.lib")
//#pragma comment(lib,"ws2_32.lib")

MYSQL *mysql;
MYSQL_RES *res;
MYSQL_ROW row;

int g_nColumnNumberCount[9] = { 9, 10, 10, 10, 10, 10, 10, 10, 11 };
char panel_patterns[3000][60];
int panel_pattern_cnt = 0;

struct st_selected_columns
{
	int npNumbers[3];
	int count;
	st_selected_columns() {
		count = 0;
		npNumbers[0] = npNumbers[1] = npNumbers[2] = 0;
	}
};

st_selected_columns selected_for_panel[9];
st_selected_columns generated_array[6][9];
st_selected_columns g_nppBingoIndexCard[6][9];

int connectDB()
{
	mysql = mysql_init(0);
	if (mysql && mysql_real_connect(mysql, "localhost", "root", "Quoted1216", "bingo", 3306, NULL, 0)==NULL)
	{
		return -1;
	}
	return 0;
}

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


// CPanelDataCreatorDlg dialog


CPanelDataCreatorDlg* dlg;

CPanelDataCreatorDlg::CPanelDataCreatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PANELDATACREATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPanelDataCreatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_CardNumber);
}

BEGIN_MESSAGE_MAP(CPanelDataCreatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPanelDataCreatorDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPanelDataCreatorDlg message handlers

BOOL CPanelDataCreatorDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	int res = connectDB();
	if (res == -1)
	{
		AfxMessageBox(_T("Can't connect to server"));
	}
	dlg = this;

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPanelDataCreatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPanelDataCreatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPanelDataCreatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int npFirstColumnNumbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int npSecondColumnNumbers[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
int npThirdColumnNumbers[] = { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };
int npFourthColumnNumbers[] = { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 };
int npFifthColumnNumbers[] = { 40, 41, 42, 43, 44, 45, 46, 47, 48, 49 };
int npSixthColumnNumbers[] = { 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
int npSeventhColumnNumbers[] = { 60, 61, 62, 63, 64,65, 66, 67, 68, 69 };
int npEighthColumnNumbers[] = { 70, 71, 72, 73, 74, 75, 76, 77, 78, 79 };
int npNinthColumnNumbers[] = { 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90 };

int npppNumberTable[6][9][3];

int nppOriginalNumbers[9][11] = {
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0},
	{ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0 },
	{ 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 0 },
	{ 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 0 },
	{ 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 0 },
	{ 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 0 },
	{ 60, 61, 62, 63, 64,65, 66, 67, 68, 69, 0 },
	{ 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0 },
	{ 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90 }
};


void swap(int& a, int& b)
{
	int tmp;
	tmp = a;
	a = b;
	b = tmp;
}

void resetSelectedColumns()
{
	for (int i = 0; i < 9; i++)
	{
		selected_for_panel[i].count = 0;
	}
}

void resetGeneratedArray()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				generated_array[i][j].npNumbers[k] = 0;
			}
			generated_array[i][j].count = 0;
		}
	}
}

void writeToGeneratedArray(int panel_id)
{
	for (int i = 0; i < 9; i++)
	{
		generated_array[panel_id][i].count = selected_for_panel[i].count;
		for (int j = 0; j < selected_for_panel[i].count; j++)
		{
			generated_array[panel_id][i].npNumbers[j] = selected_for_panel[i].npNumbers[j];
		}
	}
}

struct st_remain_array
{
	int nppArray[9][11];
	int cnt[9];
};

st_remain_array remain_array;
bool g_bContinueAbility = true;

int g_nCardNumber = 1;
int g_nPanelSerialNumber = (g_nCardNumber -1)*6;

void copyToRecoveryArray(st_remain_array &source, st_remain_array &dest)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < source.cnt[i]; j++)
		{
			dest.nppArray[i][j] = source.nppArray[i][j];
		}
		dest.cnt[i] = source.cnt[i];
	}
}

void resetRemainArray()
{
	g_bContinueAbility = true;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			remain_array.nppArray[i][j] = nppOriginalNumbers[i][j];
			remain_array.cnt[i] = 10;
			if (i == 0)
				remain_array.cnt[i] = 9;
			else if (i == 8)
				remain_array.cnt[i] = 11;
		}
	}
}

void removeFromRemainArray(int panel_id, int col_id)
{
	for (int j = 0; j < generated_array[panel_id][col_id].count; j++)
	{
		for (int i = 0; i < remain_array.cnt[col_id]; i++)
		{
			if (generated_array[panel_id][col_id].npNumbers[j] == remain_array.nppArray[col_id][i])
			{
				if(i < remain_array.cnt[col_id]-1)
					memmove(((int*)remain_array.nppArray[col_id] + i), ((int*)remain_array.nppArray[col_id] + i + 1), (remain_array.cnt[col_id] - i - 1) * sizeof(int));
				remain_array.cnt[col_id]--;
				if (panel_id < 5 && remain_array.cnt[col_id] == 0)
				{
					g_bContinueAbility = false;
					return;
				}
			}
		}
	}
// 	if (panel_id == 4)
// 		if (remain_array.cnt[panel_id] > 3)
// 		{
// 			g_bContinueAbility = false;
// 			return;
// 		}
}

void makeArrayFromRemainNumbers(int panel_id)
{
	for (int i = 0; i < 9; i++)
	{
		removeFromRemainArray(panel_id, i);
		if (g_bContinueAbility==false)
		{
			return;
		}
	}
}

void swapTableValue(int panel_id, int col_id, int idx1, int idx2)
{
	int temp = npppNumberTable[panel_id][col_id][idx1];
	npppNumberTable[panel_id][col_id][idx1] = npppNumberTable[panel_id][col_id][idx2];
	npppNumberTable[panel_id][col_id][idx2] = temp;
}
void changeBingoTableColumns(int panel_id, int col_id, int change_type)
{
	if (change_type == 1)
		return;
	if (change_type == 2)
		swapTableValue(panel_id, col_id, 1, 2);
	else if (change_type == 3)
		swapTableValue(panel_id, col_id, 0, 1);
	else if (change_type == 4)
	{
		swapTableValue(panel_id, col_id, 0, 1);
		swapTableValue(panel_id, col_id, 1, 2);
	}
	else if (change_type == 5)
		swapTableValue(panel_id, col_id, 0, 2);
	else if (change_type == 6)
	{
		swapTableValue(panel_id, col_id, 0, 2);
		swapTableValue(panel_id, col_id, 1, 2);
	}
}

void rechangeBingoTableColumns(int panel_id, int col_id, int change_type)
{
	if (change_type == 1)
		return;
	if (change_type == 2)
		swapTableValue(panel_id, col_id, 1, 2);
	else if (change_type == 3)
		swapTableValue(panel_id, col_id, 0, 1);
	else if (change_type == 4)
	{
		swapTableValue(panel_id, col_id, 1, 2);
		swapTableValue(panel_id, col_id, 0, 1);
	}
	else if (change_type == 5)
		swapTableValue(panel_id, col_id, 0, 2);
	else if (change_type == 6)
	{
		swapTableValue(panel_id, col_id, 1, 2);
		swapTableValue(panel_id, col_id, 0, 2);
	}
}

bool checkExistance(char* pattern);

bool recordToDatabase()
{
	char str[100];
	char query[300];
	for (int panel_id = 0; panel_id < 6; panel_id++)
	{
		str[0] = 0;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 9; i++)
			{
				sprintf_s(str, "%s%02d", str, npppNumberTable[panel_id][i][j]);
			}
		}
// 		if (checkExistance(str))
// 			return false;
		g_nPanelSerialNumber++;
		sprintf_s(query, "insert into panel_%d_copy (card_id, panel_id, numbers) values('%d', '%d', '%s')", panel_id + 1, g_nCardNumber, g_nPanelSerialNumber, str);
		if (mysql_query(mysql, query) != 0)
			::MessageBox(NULL, _T("Mysql query error"), _T("error"), MB_OK);
	}
	return true;

}


int g_nColNumbers[9];
struct st_combination_array
{
	int array[200][9];
	int k;
	int cnt;
	st_combination_array() {
		k = 1;
		cnt = 0;
	}
}; 

st_combination_array g_st_comb_array;

void combination(int n, int k, int idx, int depth)
{
	if (k == depth)
	{
		for (int i = 0; i < k; i++)
		{
			g_st_comb_array.array[g_st_comb_array.cnt][i] = g_nColNumbers[i];
		}
		g_st_comb_array.cnt++;
	}
	for (int i = idx; i < n; i++)
	{
		g_nColNumbers[depth] = i;
		combination(n, k, i + 1, depth + 1);
	}
}

bool checkExistance(char *pattern)
{
	for (int i = 0; i < panel_pattern_cnt; i++)
	{
		if (strcmp(pattern, panel_patterns[i]) == 0)
			return true;
	}
	return false;

}

bool checkExistanceRandom()
{
	char str[100];
	char query[1000];
	for (int panel_id = 0; panel_id < 6; panel_id++)
	{
		str[0] = 0;
		for (int j = 0; j < 3; j++)
		{
			for (int i = 0; i < 9; i++)
			{
				sprintf_s(str, "%s%02d", str, npppNumberTable[panel_id][i][j]);
			}
		}
		sprintf_s(query, "select a.numbers, b.numbers, c.numbers, d.numbers, e.numbers, f.numbers from panel_1 as a, panel_2 as b, panel_3 as c, panel_4 as d, panel_5 as e, panel_6 as f where (a.card_id=b.card_id and a.card_id=c.card_id and a.card_id=d.card_id and a.card_id=e.card_id and a.card_id=f.card_id) and (a.numbers='%s' or b.numbers='%s' or c.numbers='%s' or d.numbers='%s' or e.numbers='%s' or f.numbers='%s')", str, str, str, str, str, str);
		if (mysql_query(mysql, query) == 0)
		{
			MYSQL_RES* res = mysql_store_result(mysql);
			if (res->row_count > 0)
			{
				mysql_free_result(res);
				return true;
			}
			mysql_free_result(res);
		}
		else
			::MessageBox(NULL, _T("Mysql query error"), _T("error"), MB_OK);
	}
	return false;

}

void makeRealTableFromGenerateArray()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				npppNumberTable[i][j][k] = generated_array[i][j].npNumbers[k];
			}
		}
	}
}

void permutation(int panel_id, int col_id, int n, int k, int depth)
{
// 	int* arr = remain_array.nppArray[col_id];
	int arr[11];
	for (int i = 0; i < remain_array.cnt[col_id]; i++)
	{
		arr[i] = remain_array.nppArray[col_id][i];
	}
	if (k == depth)
	{
		if (col_id == 8)
		{
			writeToGeneratedArray(panel_id);
			st_remain_array recovery_array;
			copyToRecoveryArray(remain_array, recovery_array);
			if (panel_id < 5)
			{
				makeArrayFromRemainNumbers(panel_id);
				if (g_bContinueAbility)
				{
					resetSelectedColumns();
					if (panel_id == 3)
						int kk = 0;
					if (panel_id==4 && remain_array.cnt[0] == 3)
						permutation(panel_id + 1, 0, 3, 3, 0);
					else
					{
						int min_num = min(3, remain_array.cnt[0] - (6 - (panel_id + 1) - 1));
						int max_num = max(1, remain_array.cnt[0] - (6 - (panel_id + 1) - 1) * 3);//6-(panel_id+1)-1: remain panel count after next panel
						for (int i = min_num; i >= max_num; i--)
						{
							selected_for_panel[0].count = 0;
							permutation(panel_id + 1, 0, remain_array.cnt[0], i, 0);
						}
					}
					copyToRecoveryArray(recovery_array, remain_array);
				}
				else
				{
					copyToRecoveryArray(recovery_array, remain_array);
					g_bContinueAbility = true;
					return;
				}
			}
			else
			{
				makeRealTableFromGenerateArray();
// 				bool bNoExist = checkExistance();
// 				if (bNoExist)
// 					makeBingoTable();
				copyToRecoveryArray(recovery_array, remain_array);
				return;
			}
		}
		else
		{
			if (panel_id == 4 && remain_array.cnt[col_id + 1] == 3)
				permutation(panel_id, col_id + 1, 3, 3, 0);
			else
			{
				int min_num = min(3, remain_array.cnt[col_id + 1] - (6 - panel_id - 1));
				int max_num = max(1, remain_array.cnt[col_id + 1] - (6 - panel_id - 1) * 3);//6-(panel_id)-1: remain panel count
				for (int i = min_num; i >= max_num; i--)
				{
					selected_for_panel[col_id + 1].count = 0;
					permutation(panel_id, col_id + 1, remain_array.cnt[col_id + 1], i, 0);
				}
			}
		}
	}
	else
	{
		for (int i = depth; i < n; i++)
		{
			if(i!=depth)
				int kk = 0;
			selected_for_panel[col_id].npNumbers[depth] = arr[i];
			selected_for_panel[col_id].count = depth+1;
			swap(remain_array.nppArray[col_id][i], remain_array.nppArray[col_id][depth]);
// 			if(depth == 0 && col_id==0)
// 				copyToRecoveryArray(remain_array, recovery_array);
			permutation(panel_id, col_id, n, k, depth + 1);
			swap(remain_array.nppArray[col_id][i], remain_array.nppArray[col_id][depth]);
// 			selected_for_panel[col_id].count--;
		}
	}
}


UINT start_permutation()
{
	for (int i = 3; i > 0; i++)
	{
		resetRemainArray();
		permutation(0, 0, 9, i, 0);
		resetSelectedColumns();
	}
	return 0;
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

void make_panel()
{
	for (int i = 0; i < 9; i++)
	{
		int prev_numbers[11] = { 0 };
		int prev_number_cnt = 0;
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (g_nppBingoIndexCard[j][i].npNumbers[k] == 1)
				{
					time_t current_time;
					time(&current_time);
					int val = 0;
					bool bExist = false;
					do 
					{
						if(i > 0)
							val = (rand() + current_time) % g_nColumnNumberCount[i] + 10 * i;
						else
							val = (rand() + current_time) % g_nColumnNumberCount[i] + 1;
						bExist = checkInNumberArr(prev_numbers, prev_number_cnt, val);
						if (!bExist)
						{
							prev_numbers[prev_number_cnt++] = val;
							generated_array[j][i].npNumbers[k] = val;
						}
					} while (bExist);
				}
			}
		}
	}
}


void resetBingoIndexCard()
{
	memset(g_nppBingoIndexCard, 0, 6 * 9 * (sizeof(st_selected_columns)));
}


bool checkPanelUsability(int panel_id)
{
	for (int i = 0; i < 9; i++)
	{
		int sum = g_nppBingoIndexCard[panel_id][i].count;
// 		for (int j = 0; j < 3; j++)
// 		{
// 			sum += g_nppBingoIndexCard[panel_id][i].npNumbers[j];
// 		}
		if (sum == 0)
			return false;
	}
	return true;
}


bool checkCardUsability(bool final_check=true, int sp_panel_id=5)
{
	for (int i = 0; i < 9; i++)
	{
		int sum = 0;
		for (int panel_id = 0; panel_id < 6; panel_id++)
		{
			for (int j = 0; j < 3; j++)
			{
				sum += g_nppBingoIndexCard[panel_id][i].npNumbers[j];
			}
		}

		int max_num = g_nColumnNumberCount[i] - (6 - sp_panel_id - 1);
		int min_num = g_nColumnNumberCount[i] - (6 - sp_panel_id - 1) * 3;//6-(panel_id)-1: remain panel count
		if (sum == 0 || sum < min_num || sum > max_num)
			return false;
		if (!final_check)
		{
			if (i == 0 && sum > 9)
				return false;
			if (i == 8 && sum > 11)
				return false;
			if (i!= 0 && i!=8 && sum > 10)
				return false;
		}
		else if (final_check)
		{
			if (i == 0 && sum != 9)
				return false;
			if (i == 8 && sum != 11)
				return false;
			if (i != 0 && i != 8 && sum != 10)
				return false;
		}
	}
	return true;
}

bool checkPreRowAbility(int panel_id, int row_id)
{
	if (panel_id == 0 || panel_id == 1)
		return true;
	int row_item_cnt = 0;
	for (int i = 0; i < 9; i++)
	{
		row_item_cnt += g_nppBingoIndexCard[panel_id][i].npNumbers[row_id];
		if (row_id==2 && g_nppBingoIndexCard[panel_id][i].count == 0)
			return false;
		int sum = 0;
		for (int id = 0; id < panel_id; id++)
		{
			sum += g_nppBingoIndexCard[id][i].count;
		}
		int max_num = g_nColumnNumberCount[i] - (6 - (panel_id-1) - 1);
		int min_num = max(panel_id, g_nColumnNumberCount[i] - (6 - (panel_id-1) - 1) * 3);//6-(panel_id)-1: remain panel count
		for (int id = 0; id < row_id; id++)
		{
			sum += g_nppBingoIndexCard[panel_id][i].npNumbers[id];
		}
// 		if (sum <= min_num || sum >= max_num)
// 		{
			if (panel_id>2 && ((g_nColumnNumberCount[i]-sum) >= ((6-panel_id-1)*3+(3-row_id)))&& g_nppBingoIndexCard[panel_id][i].npNumbers[row_id] == 0)
				return false;
			if (sum > max_num && g_nppBingoIndexCard[panel_id][i].npNumbers[row_id] == 1)
				return false;
// 		}
	}
	if (row_item_cnt > 5)
		int kk = 0;
	return true;
}

void makeBingoTable()
{
	int cnt = 0;
	while (cnt < 1)
	{
		if (g_nCardNumber == 8026)
			int kk = 0;
		resetGeneratedArray();
		make_panel();
		makeRealTableFromGenerateArray();
// 		if (checkExistanceRandom()==false)
// 		{
			bool bres = recordToDatabase();
			if (bres) {
				TCHAR ss[30];
				_stprintf_s(ss, _T("%d"), g_nCardNumber);
				dlg->m_CardNumber.SetWindowText(ss);
				g_nCardNumber++;
				cnt++;
			}

// 		}
	}
}

void generate_bingo_index_card_random(int panel_id)
{
	// 	int r = rand() % 30;
	if (panel_id == 2)
		int kk = 0;
	int r = 0;
	if (panel_id < 6)
	{
		int try_cnt0 = 0;
		while (true)
		{
			bool bCardUsability = false;
			int comb_id0 = rand() % g_st_comb_array.cnt;
			try_cnt0++;
			for (int col_id0 = 0; col_id0 < g_st_comb_array.k; col_id0++)
			{
				g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].npNumbers[0] = 1;
				g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].count++;
			}
			bool bCheck = checkPreRowAbility(panel_id, 0);
			if (!bCheck)
			{
				for (int col_id0 = 0; col_id0 < g_st_comb_array.k; col_id0++)
				{
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].npNumbers[0] = 0;
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].count--;
				}
				if (try_cnt0>=10)
					break;
				else
					continue;
			}
			int try_cnt1 = 0;
			while (true)
			{
				try_cnt1++;
				int comb_id1 = rand() % g_st_comb_array.cnt;
				for (int col_id1 = 0; col_id1 < g_st_comb_array.k; col_id1++)
				{
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].npNumbers[1] = 1;
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].count++;
				}
				bool bCheck = checkPreRowAbility(panel_id, 1);
				if (!bCheck)
				{
					for (int col_id1 = 0; col_id1 < g_st_comb_array.k; col_id1++)
					{
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].npNumbers[1] = 0;
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].count--;
					}
					if(try_cnt1>=10)
						break;
					else
						continue;
				}
				int try_cnt2 = 0;
				while (true)
				{
					try_cnt2++;
					int comb_id2 = rand() % g_st_comb_array.cnt;
					for (int col_id2 = 0; col_id2 < g_st_comb_array.k; col_id2++)
					{
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].npNumbers[2] = 1;
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].count++;
					}
					bool bCheck = checkPreRowAbility(panel_id, 2);
					if (!bCheck)
					{
						for (int col_id2 = 0; col_id2 < g_st_comb_array.k; col_id2++)
						{
							g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].npNumbers[2] = 0;
							g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].count--;
						}
						if (try_cnt2 < 10)
							continue;
						else
							break;
					}
					bool bUsability = checkPanelUsability(panel_id);
					if (bUsability)
					{
						if (panel_id == 1)
							int kk = 0;
						bCardUsability = checkCardUsability(false, panel_id);
						if (bCardUsability)
							generate_bingo_index_card_random(panel_id + 1);
					}
					for (int col_id2 = 0; col_id2 < g_st_comb_array.k; col_id2++)
					{
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].npNumbers[2] = 0;
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].count--;
					}
					if (bCardUsability)
						break;
				}
				for (int col_id1 = 0; col_id1 < g_st_comb_array.k; col_id1++)
				{
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].npNumbers[1] = 0;
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].count--;
				}
				if(bCardUsability)
					break;
			}
			for (int col_id0 = 0; col_id0 < g_st_comb_array.k; col_id0++)
			{
				g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].npNumbers[0] = 0;
				g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].count--;
			}
			if(panel_id!=0 && bCardUsability)
				break;
		}
	}
	else
	{
		bool bCardUsability = checkCardUsability(true);
		panel_pattern_cnt = 0;
		if (g_nCardNumber == 8026)
			int kk = 0;
		if (bCardUsability)
			makeBingoTable();
		if (g_nCardNumber > 416668)
		{
			::MessageBox(NULL, _T("Completed"), _T("Success"), MB_OK);
		}
		if (bCardUsability)
			int kk = 0;
	}
}

void generate_bingo_index_card(int panel_id)
{
// 	int r = rand() % 30;
	if (panel_id == 2)
		int kk = 0;
	int r = 0;
	if(panel_id < 6)
		for (int comb_id0 = r; comb_id0 < g_st_comb_array.cnt; comb_id0++)
		{
			for (int col_id0 = 0; col_id0 < g_st_comb_array.k; col_id0++)
			{
				g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].npNumbers[0] = 1;
				g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].count++;
			}
			bool bCheck = checkPreRowAbility(panel_id, 0);
			if (!bCheck)
			{
				for (int col_id0 = 0; col_id0 < g_st_comb_array.k; col_id0++)
				{
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].npNumbers[0] = 0;
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].count--;
				}
				continue;
			}
// 			r = rand() % 30;
			for (int comb_id1 = r; comb_id1 < g_st_comb_array.cnt; comb_id1++)
			{
				for (int col_id1 = 0; col_id1 < g_st_comb_array.k; col_id1++)
				{
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].npNumbers[1] = 1;
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].count++;
				}
				bool bCheck = checkPreRowAbility(panel_id, 1);
				if (!bCheck)
				{
					for (int col_id1 = 0; col_id1 < g_st_comb_array.k; col_id1++)
					{
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].npNumbers[1] = 0;
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].count--;
					}
					continue;
				}
				// 				r = rand() % 30;
				for (int comb_id2 = r; comb_id2 < g_st_comb_array.cnt; comb_id2++)
				{
					for (int col_id2 = 0; col_id2 < g_st_comb_array.k; col_id2++)
					{
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].npNumbers[2] = 1;
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].count++;
					}
					bool bCheck = checkPreRowAbility(panel_id, 2);
					if (!bCheck)
					{
						for (int col_id2 = 0; col_id2 < g_st_comb_array.k; col_id2++)
						{
							g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].npNumbers[2] = 0;
							g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].count--;
						}
						continue;
					}
					bool bUsability = checkPanelUsability(panel_id);
					if (bUsability)
					{
						if (panel_id == 1)
							int kk = 0;
						bool bCardUsability = checkCardUsability(false, panel_id);
						if(bCardUsability)
							generate_bingo_index_card(panel_id + 1);
					}
					for (int col_id2 = 0; col_id2 < g_st_comb_array.k; col_id2++)
					{
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].npNumbers[2] = 0;
						g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id2][col_id2]].count--;
					}
				}
				for (int col_id1 = 0; col_id1 < g_st_comb_array.k; col_id1++)
				{
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].npNumbers[1] = 0;
					g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id1][col_id1]].count--;
				}
			}
			for (int col_id0 = 0; col_id0 < g_st_comb_array.k; col_id0++)
			{
				g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].npNumbers[0] = 0;
				g_nppBingoIndexCard[panel_id][g_st_comb_array.array[comb_id0][col_id0]].count--;
			}

		}
	else
	{
		bool bCardUsability = checkCardUsability(true);
		panel_pattern_cnt = 0;
		if (g_nCardNumber == 8026)
			int kk = 0;
		if(bCardUsability)
			makeBingoTable();
		if (g_nCardNumber > 416667)
		{
			::MessageBox(NULL, _T("Completed"), _T("Success"), MB_OK);
		}
		if (bCardUsability)
			int kk = 0;
	}
}

UINT random_generate()
{
	g_st_comb_array.k = 5;
	combination(9, 5, 0, 0);
	generate_bingo_index_card_random(0);
	return 0;
}

void CPanelDataCreatorDlg::OnBnClickedOk()
{
	AfxBeginThread((AFX_THREADPROC )random_generate, 0, THREAD_PRIORITY_NORMAL);
}
