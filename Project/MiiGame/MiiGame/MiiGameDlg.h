// MiiGameDlg.h : header file
//

#pragma once

#include "DriveControl.h"
#include "afxwin.h"
#include "ImageEntry.h"
#include <vector>
#include "afxcmn.h"
using namespace std;
// CMiiGameDlg dialog
class CMiiGameDlg : public CDialog
{
// Construction
public:
	CMiiGameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MIIGAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
public:
    static void W2MB(const CString& a, CStringA& b);
private:
    CDriveControl m_driveControl;
    vector<CImageEntry> m_vecImageEntries;
    vector<CImageEntry> m_vecISOEntries;
    CListCtrl m_diskList;
    CListCtrl m_isoList;
    CComboBox m_driveCombo;
private:
    bool OpenDrive();
    void GetDisks();
    void SetInformationToList();
// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
    DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedLoadBtn();
    afx_msg void OnBnClickedUpdateDriveBtn();
    afx_msg void OnBnClickedOpenFileBtn();
};
