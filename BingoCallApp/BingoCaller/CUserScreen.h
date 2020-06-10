#pragma once


// #include "BingoCallerDlg.h"

class CUserScreen : public CDialogEx
{
	DECLARE_DYNAMIC(CUserScreen)

public:
	CUserScreen(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CUserScreen();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USER_SCREEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CDialog* m_pParentWnd;
	virtual BOOL OnInitDialog();
	void OnDraw(CDC* dc);
	void makeNumberPanel();

public:
	CRect m_nNumPanelRect;
	CRect m_LastNumberRect;
	CRect m_VerifyPanelRect;
	CRect m_LogoPanelRect;
	CRect m_GameNumberRect;
	CRect m_colorRect;
	CRect m_advertiseRect;
	CRect m_GameLevelRect;
	CRect m_NumbersCalledRect;

	int m_nCalledLastNumber;
	COLORREF m_color;
	TCHAR m_szColor[20];
	int m_nNumbersCalled;
	int m_nGameNumber;
	int m_nGameLevel;
	TCHAR m_szLogoFile[MAX_PATH];
	TCHAR m_szAdFile[MAX_PATH];
	TCHAR m_szAdText[200];

	CImage m_imgLogo;
	CImage m_imgAd;

	bool m_bVerified;
	CArray<int> m_arrOneLineVerified;
	CArray<int> m_arrTwoLineVerified;
	CArray<int> m_arrFullHouseVerified;

	int m_verifiedPanel[9][3];
	int m_nverifiedPanelId;

	void resetNumberArray();
	void drawYellowBoxes(CDC* dc);
	void setPanelMatchInfo(int number, int bmatch);
	void resetPanel();
	void setPanelRect();
	void addToVerified(int panel_id, int level);
	void resetVerifiedArray();
	TCHAR m_szVerifiedString[3000];
};
