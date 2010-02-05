// MiiGameDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DriveControl.h"
#include "ImageEntry.h"
#include "EntryListCtrl.h"
#include "NotifyBase.h"
#include "../Localization/Localization.h"
#include <vector>
#include <map>

using namespace std;
// CMiiGameDlg dialog
class CMiiGameDlg : public CDialog, public CINotifyBase
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
    void Event(const TSTRING& strEvent,long nParam);
    
private:
    map<int, CString> m_errorMSg;
    CDriveControl m_driveControl;
    vector<CImageEntry> m_vecDiskEntries;
    vector<CImageEntry> m_vecISOEntries;
    CEntryListCtrl m_diskList;
    CEntryListCtrl m_isoList;
    CComboBox m_driveCombo;
    CComboBox m_folderDropList;
    CLocalization m_localization;
    CStatusBar m_bar;

private:
    bool OpenDrive();
    void GetDisks();
    void SetInformationToList();
    void SetImageListContent(bool bCleanOriginalList);
    void InitListCcontrol();
    void OnEventUploadError(long nParam);
    void OnEventDownloadError(long nParam);
    void UpdateUIString(LCID lcid);
    void RenameDisk( long nParam );

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
    afx_msg void OnBnClickedUploadBtn();
    afx_msg void OnBnClickedDownloadBtn();
    afx_msg void OnBnClickedFormatBtn();
    afx_msg void OnBnClickedDeleteBtn();
    afx_msg void OnBnClickedDeleteBtn2();
    afx_msg void OnBnClickedButton2();
};
