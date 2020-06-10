
// BingoCallerDlg.h : header file
//

#include ".\Control\ButtonControl.h"
#include "CUserScreen.h"
#include <WinSock2.h>


#pragma once

struct st_recall_info
{
	int call_id;
	int called_number;
};

struct st_panel_verified_info
{
	int panel_id;
	bool bVerified;
};

struct st_verified_info
{
	st_panel_verified_info panel_info[5000];
	int cnt;
};

// CBingoCallerDlg dialog
class CBingoCallerDlg : public CDialogEx
{
// Construction
public:
	CBingoCallerDlg(CWnd* pParent = nullptr);	// standard constructor
	~CBingoCallerDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINGOCALLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


// Implementation
protected:
	HICON m_hIcon;

	CButtonControl m_cStartButtonControl;
	CButtonControl m_cStopButtonControl;
	CButtonControl m_cCallButtonControl;
	CButtonControl m_cRecallButtonControl;
	CButtonControl m_cVerifyButtonControl;
	TCHAR m_szNumberCount[10];

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CUserScreen* m_userScreen;
	void CreateButtons();
	void OnDraw(CDC* dc);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void makeNumberPanel();
	int m_nCalledCount;
	int m_nCalledLastNumber;
	void resetNumberArray();
	bool m_bStopped;
	CRect m_calledCountRect;
	CRect m_LastCalledNumberRect;
	CRect m_nNumPanelRect;
	
	int m_npCalledNumbers[90];
	int resetCalledNumbersArray();
	bool checkInCalledNumbers(int num);
	afx_msg void OnBnClickedButtonCheck();
	int m_nPanelNumber;
	int m_nppWinnerPanelInfo[6][9][3];
	int m_npWinnerCardSerialInfo[6];
	void writeToWinnerPanelInfo(char*panel_id1, char* panel_numbers1, char* panel_id2, char* panel_numbers2, char* panel_id3, char* panel_numbers3, char* panel_id4, char* panel_numbers4, char* panel_id5, char* panel_numbers5, char* panel_id6, char* panel_numbers6);
	int checkFullHouse(int panel_id);
	int m_npMatchedPanels[50];
	int m_nMatchedPanelCnt;
	bool verifyLastMatched();
	int m_npLastMatchedPanels[50];
	int m_nLastMatchedPanelCnt;
	int m_npFinalWinners[50];
	int m_nFinalWinnersCnt;
	bool checkLastNumberInWinningPanel(int panel_id);
	afx_msg void OnBnClickedOk();
	bool getPanelRange();
	int m_nFromPanel;
	int m_nToPanel;
	CString m_szOneLine;
	CString m_szTwoLine;
	CString m_szFullHouse;
	void getPanelRangeInfo();
	void resetPanelInfo();
	int checkPanelInfo(int number, int level);
	int m_nCurrMatchLevel;
	bool m_bNumberVerified;
	int m_nVerifiedCnt;
	int m_nPanelRangeFrom;
	int m_nPanelRangeTo;
	void recordCallingAction(st_recall_info recall_info);
	void RecallAction();
	bool CallAction();
	afx_msg void OnBnClickedButtonSetRange();
	void drawYellowBoxes(CDC* dc);
	CString m_szOneLineMiss;
	CString m_szTwoLineMiss;
	CString m_szFullHouseMiss;
	st_verified_info m_VerifiedInfo;
	CArray<st_panel_verified_info> m_onelineMatchedInfo;
	CArray<st_panel_verified_info> m_twolineMatchedInfo;
	CArray<st_panel_verified_info> m_fullhouseMatchedInfo;
	st_verified_info m_onelineMissedInfo;
	st_verified_info m_twolineMissedInfo;
	st_verified_info m_fullhouseMissedInfo;
	void addToVerifiedInfo();
	void checkAsVerified(int panel_id);
	void moveUnverifiedPanelsToMissedMatch();
	bool checkInMissedPattern(int panel_id);
	BOOL m_bOneLineWinner;
	BOOL m_bTwoLineWinner;
	afx_msg void OnBnClickedCheckOneline();
	afx_msg void OnBnClickedCheckTwoline();
	void copyVerifiedInfo(int currLevel);
	void makeWinnerString(TCHAR* str);
	void rearrangeVerifiedArrayInfo();
	void initializeServer();
	void setColorInfo();
	TCHAR m_currColorStr[20];
	COLORREF getRGBFromColorString(TCHAR *color_str);
	int m_nGameNumber;
	bool m_bCanStartNewGameWithNumber;
	void loadPanelInfo();
	void createUserScreen();
	CString m_szLogoFilename;
	CString m_szAdText;
	afx_msg void OnBnClickedButtonLogo();
	CString m_szAdFilename;
	afx_msg void OnBnClickedButtonAdvertise();
	afx_msg void OnBnClickedButtonAdTextSet();
	void resetUserScreen();
	void loadPanelInfoForRange();
	void loadPanelInfoForTicket();
	afx_msg void OnEnChangeEditRangeFrom();
	afx_msg void OnEnChangeEditRangeTo();
	afx_msg void OnBnClickedButtonAdImageRemove();
};
