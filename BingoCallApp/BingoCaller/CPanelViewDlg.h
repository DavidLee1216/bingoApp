#pragma once


// CPanelViewDlg dialog

class CPanelViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPanelViewDlg)

public:
	CPanelViewDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPanelViewDlg();

	CDialog* m_pParentWnd;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDC* m_pPrintDC;
	LPPRINTDLG m_pPD;
	int m_nPanelId;

	virtual BOOL OnInitDialog();
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);

	void OnPrintDraw(CDC* pDC, CPrintInfo* pInfo);


	afx_msg void OnPaint();
	void OnDraw(CDC* dc);
	void resetPanelCallState();
	int getWinnedLines(int panel_id);
	afx_msg void OnBnClickedPrint();
	CDC* CreatePrintDC();
	void print();
	afx_msg void OnIdclose();
};
