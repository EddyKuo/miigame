// MiiGameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiiGame.h"
#include "MiiGameDlg.h"
#include "../libwbfsNETwrapper/WbfsInterm.h"
#include "shlobj.h"
#include <string>
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

TCHAR* szTitle = _T("Mii Game for your games");

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMiiGameDlg dialog

static CMiiGameDlg* pCMiiGameDlg = NULL;

CMiiGameDlg::CMiiGameDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMiiGameDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    pCMiiGameDlg = this;
    //m_driveControl.Register(this, EVENT_UPLOAD_COMPLETE);
    //m_driveControl.Register(this, EVENT_UPLOAD_PROGRESS);
    m_driveControl.Register(this, EVENT_UPLOAD_ALL_COMPLETE);
    m_driveControl.Register(this, EVENT_UPLOAD_FAILED);

    //m_driveControl.Register(this, EVENT_DOWNLOAD_COMPLETE);
    //m_driveControl.Register(this, EVENT_DOWNLOAD_PROGRESS);
    m_driveControl.Register(this, EVENT_DOWNLOAD_ALL_COMPLETE);
    m_driveControl.Register(this, EVENT_DOWNLOAD_FAILED);

    m_isoList.Register(this, EVENT_ZERO_LENGTH_EDITION);
    m_diskList.Register(this, EVENT_CHANGE_DISK_NAME);
}

void CMiiGameDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO2, m_driveCombo);
    DDX_Control(pDX, IDC_LIST3, m_diskList);
    DDX_Control(pDX, IDC_LIST4, m_isoList);
    DDX_Control(pDX, IDC_FOLDER_COMBO, m_folderDropList);
}

BEGIN_MESSAGE_MAP(CMiiGameDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_LOAD_BTN, &CMiiGameDlg::OnBnClickedLoadBtn)
    ON_BN_CLICKED(IDC_UPDATE_DRIVE_BTN, &CMiiGameDlg::OnBnClickedUpdateDriveBtn)
    ON_BN_CLICKED(IDC_OPEN_FILE_BTN, &CMiiGameDlg::OnBnClickedOpenFileBtn)
    ON_BN_CLICKED(IDC_UPLOAD_BTN, &CMiiGameDlg::OnBnClickedUploadBtn)
    ON_BN_CLICKED(IDC_DOWNLOAD_BTN, &CMiiGameDlg::OnBnClickedDownloadBtn)
    ON_BN_CLICKED(IDC_FORMAT_BTN, &CMiiGameDlg::OnBnClickedFormatBtn)
    ON_BN_CLICKED(IDC_DELETE_BTN, &CMiiGameDlg::OnBnClickedDeleteBtn)
    ON_BN_CLICKED(IDC_DELETE_BTN2, &CMiiGameDlg::OnBnClickedDeleteBtn2)
    ON_BN_CLICKED(IDC_BUTTON2, &CMiiGameDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMiiGameDlg message handlers

BOOL CMiiGameDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetWindowText(szTitle);
    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
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

    OnBnClickedUpdateDriveBtn();
    InitListCcontrol();

    static UINT BASED_CODE indicators[] = {
        IDS_INDICATOR_INFO,
        IDS_INDICATOR_TIME
    };
    m_bar.Create(this); //We create the status bar
    m_bar.SetIndicators(indicators,2); //Set the number of panes 
    CRect rect;
    GetClientRect(&rect);

    m_bar.SetPaneInfo(0, IDS_INDICATOR_INFO, SBPS_NORMAL,rect.Width()-100);
    m_bar.SetPaneInfo(1, IDS_INDICATOR_TIME, SBPS_STRETCH ,0);

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, IDS_INDICATOR_TIME);
    //m_bar.GetStatusBarCtrl().SetBkColor(RGB(180,180,180));
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMiiGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMiiGameDlg::OnPaint()
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
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMiiGameDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMiiGameDlg::Event(const TSTRING& strEvent,long nParam) {
    if(strEvent == EVENT_UPLOAD_COMPLETE) {
        OnEventUploadError(nParam);
    } else if(strEvent == EVENT_UPLOAD_PROGRESS) {
        //int nStatus = nParam >> 16, nTotal = nParam & 0xFFFF;
        //int nPercentage = (int) (((float)nStatus) / ((float)nTotal) * 100);
        //m_progress.SetPos(nPercentage);
        //m_progress.UpdateWindow();
    } else if(strEvent == EVENT_DOWNLOAD_COMPLETE) {
        OnEventDownloadError(nParam);
    } else if(strEvent == EVENT_DOWNLOAD_PROGRESS) {
        //int nStatus = nParam >> 16, nTotal = nParam & 0xFFFF;
        //int nPercentage = (int) (((float)nStatus) / ((float)nTotal) * 100);
        //m_progress.SetPos(nPercentage);
        //m_progress.UpdateWindow();
    } else if(strEvent == EVENT_UPLOAD_ALL_COMPLETE) {
        AfxMessageBox(m_localization.GetIDString(_T("IDS_Upload_to_WBFS_successfully")), MB_OK);
        CloseDrive();
        OnBnClickedLoadBtn();
    } else if(strEvent == EVENT_DOWNLOAD_ALL_COMPLETE) {
        AfxMessageBox(m_localization.GetIDString(_T("IDS_Download_successfully")), MB_OK);
        CloseDrive();
    } else if(strEvent == EVENT_UPLOAD_FAILED) {
        CloseDrive();
    } else if(strEvent == EVENT_DOWNLOAD_FAILED) {
        CloseDrive();
    } else if(strEvent == EVENT_ZERO_LENGTH_EDITION) {
        SetImageListContent(true);
    } else if(strEvent == EVENT_CHANGE_DISK_NAME) {
        RenameDisk(nParam);
    }
}

void CMiiGameDlg::InitListCcontrol()
{
    LONG lStyle = GetWindowLong(m_diskList.m_hWnd, GWL_STYLE); // Get the current window style 
    lStyle |= LVS_EDITLABELS | LVS_SHOWSELALWAYS; // set style 
    SetWindowLong(m_diskList.m_hWnd, GWL_STYLE, lStyle); // set style
    lStyle &= ~LVS_EDITLABELS;
    SetWindowLong(m_isoList.m_hWnd, GWL_STYLE, lStyle); // set style

    m_diskList.InsertColumn(0, m_localization.GetIDString(_T("IDS_GAME_TITLE")), 0, 170);
    m_diskList.InsertColumn(1, m_localization.GetIDString(_T("IDS_GAME_ID")), 0, 60);
    m_diskList.InsertColumn(2, m_localization.GetIDString(_T("IDS_IMAGE_SIZE")), 0, 60);
    m_diskList.InsertColumn(3, m_localization.GetIDString(_T("IDS_REGION_CODE")), 0, 50);
    m_diskList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);    

    m_isoList.InsertColumn(0, m_localization.GetIDString(_T("IDS_GAME_TITLE")), 0, 170);
    m_isoList.InsertColumn(1, m_localization.GetIDString(_T("IDS_IMAGE_SIZE")), 0, 60);
    m_isoList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CMiiGameDlg::OnBnClickedLoadBtn()
{
    // Load this driver letters that combobox indicated.
    if(!OpenDrive()) return;
    GetDisks();
    SetInformationToList();
    unsigned int blocks;
    float total;
    float used;
    float free;
    m_driveControl.GetDriveStatus(blocks, total, used, free);
    CString strFormetter;
    strFormetter.Format(_T("%.2fG"), total);
    GetDlgItem(IDC_TOTAL_SPACE)->SetWindowText(strFormetter);
    strFormetter.Format(_T("%.2fG"), used);
    GetDlgItem(IDC_USED_SPACE)->SetWindowText(strFormetter);
    strFormetter.Format(_T("%.2fG"), free);
    GetDlgItem(IDC_FREE_SPACE)->SetWindowText(strFormetter);
    CloseDrive();
    GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
}

void CMiiGameDlg::OnBnClickedUpdateDriveBtn()
{
    // Update the drivers
    while(true) {
        if(m_driveCombo.GetCount() == 0) break;
        m_driveCombo.DeleteString(0);
    }
    m_driveControl.UpdateDrivers();
    for(int i = 0; i < m_driveControl.GetDrivers().size(); ++i) {
        m_driveCombo.AddString(m_driveControl.GetDrivers().at(i));
    }
    m_driveCombo.SetCurSel(0);
    GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
}

bool CMiiGameDlg::OpenDrive()
{
    CString strDriveLetter;
    m_driveCombo.GetWindowText(strDriveLetter);
    CStringA straDriveLetter;
    straDriveLetter.Format("%S", strDriveLetter.GetString());
    straDriveLetter.Replace(":", "");
    return m_driveControl.OpenDrive(straDriveLetter);
}

void CMiiGameDlg::GetDisks() {
    m_vecDiskEntries.clear();
    CString strDriveLetter;
    m_driveCombo.GetWindowText(strDriveLetter);
    CStringA straDriveLetter;
    straDriveLetter.Format("%S", strDriveLetter.GetString());
    straDriveLetter.Replace(":", "");
    m_vecDiskEntries = m_driveControl.GetDiskFromDrive(straDriveLetter);
}

void CMiiGameDlg::SetInformationToList()
{
    m_diskList.SetRedraw(FALSE);
    m_diskList.DeleteAllItems();
    for(int i = 0; i < m_vecDiskEntries.size(); ++i) {
        CString strDiskName, strDiskID;
        strDiskName.Format(_T("%S"), m_vecDiskEntries.at(i).m_strDiskName.GetString());
        m_diskList.InsertItem(i, strDiskName);
        strDiskID.Format(_T("%S"), m_vecDiskEntries.at(i).m_strDiskID.GetString());
        m_diskList.SetItemText(i, 1, strDiskID);
        CString strSize;
        strSize.Format(_T("%.2f GB"), m_vecDiskEntries.at(i).m_fDiskSize);
        m_diskList.SetItemText(i, 2, strSize);

        m_diskList.SetItemText(i, 3, m_vecDiskEntries.at(i).m_strRegionCode);
        //m_diskList.SetItemText(i, 3, m_vecImageEntries.at(i).m_strDiskID.GetString());
    }
    m_diskList.SetRedraw(TRUE);
}

void CMiiGameDlg::SetImageListContent(bool bCleanOriginalList)
{
    m_isoList.SetRedraw(FALSE);
    if(bCleanOriginalList)
        m_isoList.DeleteAllItems();
    for(int i = 0; i < m_vecISOEntries.size(); ++i) {
        CString strFileName;
        MB2W(m_vecISOEntries.at(i).m_strDiskName, strFileName);
        m_isoList.InsertItem(i, strFileName);
        CString strSize;
        strSize.Format(_T("%.2fGB"), m_vecISOEntries.at(i).m_fDiskSize);
        m_isoList.SetItemText(i, 1, strSize);
    }
    m_isoList.SetRedraw(TRUE);
}

void CMiiGameDlg::OnBnClickedOpenFileBtn()
{
    CString strFilter = _T("rar|*.rar|zip|*.zip|iso|*.iso||");
    CFileDialog fd(TRUE, 0, 0, OFN_ALLOWMULTISELECT, strFilter, this);
    if(fd.DoModal() == IDOK) {
        POSITION pos = fd.GetStartPosition();
        while(pos != NULL) {
            CString strFilePath = fd.GetNextPathName(pos);
            strFilePath.Replace(_T("\\\\"), _T("\\"));
            CImageEntry entry = m_driveControl.GetImageFromHD(strFilePath);
            bool bAdd = true;
            for(int i = 0; i < m_vecISOEntries.size(); ++i) {
                CStringA path = m_vecISOEntries.at(i).m_strImagePath;
                if(path == entry.m_strImagePath) {
                    bAdd = false;
                    break;
                }	
            }
            if(bAdd)
                m_vecISOEntries.push_back(entry);
        }
        SetImageListContent(true);
    }
}

void CMiiGameDlg::W2MB(const CString& a, CStringA& b) {
    DWORD dwNum  = WideCharToMultiByte(CP_OEMCP, 0, a.GetString(), -1, 0, 0, 0, 0);
    char *psText = new char[dwNum+2];
    memset(psText, 0, dwNum+2);
    WideCharToMultiByte (CP_OEMCP , 0, a.GetString(), -1, psText, dwNum, 0, 0);
    b = psText;
    delete [] psText;
}

void CMiiGameDlg::MB2W(const CStringA& a, CString& b) {
    DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, a.GetString(), -1, 0, 0);
    wchar_t *pwText = new wchar_t[dwNum+2];
    memset(pwText, 0, sizeof(wchar_t)*(dwNum+2));
    MultiByteToWideChar (CP_ACP, 0, a.GetString(), -1, pwText, dwNum);
    b = pwText;
    delete [] pwText;
}

void CMiiGameDlg::OnBnClickedUploadBtn()
{
    // Add disk to drive (from your HD to WBFS)
    vector<CString>* pvecSelectName = new vector<CString>();
    POSITION pos = m_isoList.GetFirstSelectedItemPosition();
    if(pos == NULL) return;
    while(pos != NULL) {
        int nItem = m_isoList.GetNextSelectedItem(pos);
        CString strItemText = m_isoList.GetItemText(nItem, 0);
        pvecSelectName->push_back(strItemText);
    }
    CString strCompleteItem;
    strCompleteItem.Format(_T(" 0/%d"), pvecSelectName->size());
    CProgressDlg pd(m_localization.GetIDString(_T("IDS_Uploading")), m_localization.GetIDString(_T("IDS_Total_Complete")) + strCompleteItem, _T("0.00"));
    pd.SetItemInformation(0, (int)pvecSelectName->size());
    m_driveControl.Register(&pd, EVENT_UPLOAD_COMPLETE);
    m_driveControl.Register(&pd, EVENT_UPLOAD_ALL_COMPLETE);
    m_driveControl.Register(&pd, EVENT_UPLOAD_PROGRESS);
    if(this->OpenDrive() != true) return;
    m_driveControl.UploadImageToWBFS(pvecSelectName, m_vecISOEntries);
    pd.DoModal();
    m_driveControl.Unregister(&pd, EVENT_UPLOAD_COMPLETE);
    m_driveControl.Unregister(&pd, EVENT_UPLOAD_ALL_COMPLETE);
    m_driveControl.Unregister(&pd, EVENT_UPLOAD_PROGRESS);
}

void CMiiGameDlg::OnBnClickedDownloadBtn()
{
    // Extract the image from the WBFS to your HD
    vector<CString>* pvecSelectName = new vector<CString>();
    POSITION pos = m_diskList.GetFirstSelectedItemPosition();
    if(pos == NULL) return;
    while(pos != NULL) {
        int nItem = m_diskList.GetNextSelectedItem(pos);
        CString strItemText = m_diskList.GetItemText(nItem, 0);
        pvecSelectName->push_back(strItemText);
    }
    CString strCompleteItem;
    strCompleteItem.Format(_T(" 0/%d"), pvecSelectName->size());
    CProgressDlg pd(m_localization.GetIDString(_T("IDS_Downloading")), m_localization.GetIDString(_T("IDS_Total_Complete")) + strCompleteItem, _T("0.00"));
    pd.SetItemInformation(0, (int)pvecSelectName->size());
    m_driveControl.Register(&pd, EVENT_DOWNLOAD_COMPLETE);
    m_driveControl.Register(&pd, EVENT_DOWNLOAD_ALL_COMPLETE);
    m_driveControl.Register(&pd, EVENT_DOWNLOAD_PROGRESS);
    if(!this->OpenDrive()) return;
    m_driveControl.ExtractDiskToHD(pvecSelectName, m_vecDiskEntries);
    pd.DoModal();
    m_driveControl.Unregister(&pd, EVENT_DOWNLOAD_COMPLETE);
    m_driveControl.Unregister(&pd, EVENT_DOWNLOAD_ALL_COMPLETE);
    m_driveControl.Unregister(&pd, EVENT_DOWNLOAD_PROGRESS);
}

void CMiiGameDlg::OnBnClickedFormatBtn()
{
    int nRet = AfxMessageBox(m_localization.GetIDString(_T("IDS_Are_you_sure_to_format_this_driver")), MB_YESNO);
    if(nRet == IDNO) return;

    CString strDriveLetter;
    m_driveCombo.GetWindowText(strDriveLetter);
    CStringA straDriveLetter;
    straDriveLetter.Format("%S", strDriveLetter.GetString());
    straDriveLetter.Replace(":", "");
    CloseDrive();
    if(!FormatDrive((char*)straDriveLetter.GetString())) {
        AfxMessageBox(m_localization.GetIDString(_T("IDS_Format_failed")), MB_ICONERROR | MB_OK);
    } else {
        AfxMessageBox(m_localization.GetIDString(_T("IDS_Format_successfully")), MB_ICONINFORMATION | MB_OK);
    }
    CloseDrive();
    OnBnClickedLoadBtn();
}


void CMiiGameDlg::OnBnClickedDeleteBtn()
{
    if(!this->OpenDrive()) return;
    vector<CString> vecSelectName;
    POSITION pos = m_diskList.GetFirstSelectedItemPosition();
    if(pos == NULL) return;
    while(pos != NULL) {
        int nItem = m_diskList.GetNextSelectedItem(pos);
        CString strItemText = m_diskList.GetItemText(nItem, 0);
        vecSelectName.push_back(strItemText);
    }
    for(int i = 0; i < vecSelectName.size(); ++i) {
        CStringA strSelectedName;
        W2MB(vecSelectName.at(i), strSelectedName);
        for(int j = 0; j < m_vecDiskEntries.size(); ++j) {
            if(m_vecDiskEntries.at(j).m_strDiskName == strSelectedName) {
                RemoveDiscFromDrive((char*)m_vecDiskEntries.at(j).m_strDiskID.GetString());
            }
        }
    }
    CloseDrive();
    OnBnClickedLoadBtn();
}

void CMiiGameDlg::OnBnClickedDeleteBtn2()
{
    vector<CString> vecSelectName;
    POSITION pos = m_isoList.GetFirstSelectedItemPosition();
    if(pos == NULL) return;
    while(pos != NULL) {
        int nItem = m_isoList.GetNextSelectedItem(pos);
        CString strItemText = m_isoList.GetItemText(nItem, 0);
        vecSelectName.push_back(strItemText);
    }

    for(int i = 0; i < vecSelectName.size(); ++i) {
        CStringA strSelectedName;
        W2MB(vecSelectName.at(i), strSelectedName);
        vector<CImageEntry>::iterator it = m_vecISOEntries.begin();
        for(; it != m_vecISOEntries.end(); it++) {
            if(it->m_strDiskName == strSelectedName) {
                m_vecISOEntries.erase(it);
                break;
            }
        }
    }
    SetImageListContent(true);
}

void CMiiGameDlg::OnEventUploadError(long nParam)
{
    if(nParam == 0) return;
    CString strMsg;
    switch(nParam) {
        case -1:
            strMsg = m_localization.GetIDString(_T("IDS_Partition_wasn't_loaded_previously"));
            break;
        case -2:
            strMsg = m_localization.GetIDString(_T("IDS_Error_occured_while_attempting_to_read_file"));
            break;
        case -3:
            strMsg = m_localization.GetIDString(_T("IDS_Disc_already_exists_on_WBFS_drive"));
            break;
    }
    AfxMessageBox(strMsg, nParam == 0 ? MB_ICONWARNING | MB_OK : MB_OK);
}

void CMiiGameDlg::OnEventDownloadError(long nParam)
{
    if(nParam == 0) return;
    CString strMsg;
    switch(nParam) {
    case -1:
        strMsg = m_localization.GetIDString(_T("IDS_Partition_wasn't_loaded_previously"));
        break;
    case -2:
        strMsg = m_localization.GetIDString(_T("IDS_File_could_not_be_found_on_WBFS_drive"));
        break;
    case -3:
        strMsg = m_localization.GetIDString(_T("IDS_Unable_to_open_file_on_disk_for_writing"));
        break;
    }
    AfxMessageBox(strMsg, nParam == 0 ? MB_ICONWARNING | MB_OK : MB_OK);
}

void CMiiGameDlg::OnBnClickedButton2()
{

}

void CMiiGameDlg::RenameDisk(long nParam)
{
    NMLVDISPINFO *pDispInfo = (NMLVDISPINFO *)nParam;
    CStringA strNewName;
    W2MB(pDispInfo->item.pszText, strNewName);
    OpenDrive();
    int nRet = RenameDiscOnDrive((char*)m_vecDiskEntries.at(pDispInfo->item.iItem).m_strDiskID.GetString(), (char*)strNewName.GetString());
    if(nRet == 0)
        m_vecDiskEntries.at(pDispInfo->item.iItem).m_strDiskName = strNewName;
    CloseDrive();
}