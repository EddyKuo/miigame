// MiiGameDlg.h : header file
//

#pragma once

#include "DriveControl.h"
#include "afxwin.h"
#include "ImageEntry.h"
#include <vector>
#include "afxcmn.h"
#include "EntryListCtrl.h"

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
    static void MB2W(const CStringA& a, CString& b);
	static void Progress(int status, int total);
private:
    CDriveControl m_driveControl;
    vector<CImageEntry> m_vecDiskEntries;
    vector<CImageEntry> m_vecISOEntries;
    CEntryListCtrl m_diskList;
    CEntryListCtrl m_isoList;
    CComboBox m_driveCombo;
    CComboBox m_folderDropList;
private:
    bool OpenDrive();
    void GetDisks();
    void SetInformationToList();
    void SetImageListContent();
    void InitListCcontrol();
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
    afx_msg void OnBnClickedMoveLeftBtn();
    afx_msg void OnBnClickedMoveRightBtn();
    afx_msg void OnBnClickedFormatBtn();
    afx_msg void OnBnClickedDeleteBtn();
};
