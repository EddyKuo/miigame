// MiiGameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiiGame.h"
#include "MiiGameDlg.h"
#include "../libwbfsNETwrapper/WbfsInterm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

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




CMiiGameDlg::CMiiGameDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMiiGameDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
    ON_BN_CLICKED(IDC_MOVE_LEFT_BTN, &CMiiGameDlg::OnBnClickedMoveLeftBtn)
    ON_BN_CLICKED(IDC_MOVE_RIGHT_BTN, &CMiiGameDlg::OnBnClickedMoveRightBtn)
    ON_BN_CLICKED(IDC_FORMAT_BTN, &CMiiGameDlg::OnBnClickedFormatBtn)
    ON_BN_CLICKED(IDC_DELETE_BTN, &CMiiGameDlg::OnBnClickedDeleteBtn)
    ON_BN_CLICKED(IDC_DELETE_BTN2, &CMiiGameDlg::OnBnClickedDeleteBtn2)
END_MESSAGE_MAP()


// CMiiGameDlg message handlers

BOOL CMiiGameDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

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

void CMiiGameDlg::InitListCcontrol()
{
    LONG lStyle = GetWindowLong(m_diskList.m_hWnd, GWL_STYLE); // Get the current window style 
    lStyle |= LVS_EDITLABELS | LVS_SHOWSELALWAYS; // set style 
    SetWindowLong(m_diskList.m_hWnd, GWL_STYLE, lStyle); // set style
    SetWindowLong(m_isoList.m_hWnd, GWL_STYLE, lStyle); // set style

    m_diskList.InsertColumn(0, _T("Game Title"), 0, 170);
    m_diskList.InsertColumn(1, _T("Game ID"), 0, 60);
    m_diskList.InsertColumn(2, _T("Image Size"), 0, 60);
    //m_diskList.InsertColumn(3, _T("Region Code"), 0, 50);
    m_diskList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);    

    m_isoList.InsertColumn(0, _T("Game Title"), 0, 170);
    m_isoList.InsertColumn(2, _T("Image Size"), 0, 60);
    m_isoList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CMiiGameDlg::OnBnClickedLoadBtn()
{
    // Load this driver letters that combobox indicated.
    if(!OpenDrive()) return;
    GetDisks();
    SetInformationToList();
    CloseDrive();
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
        pos = fd.GetStartPosition();
        CString strFilePath = fd.GetNextPathName(pos);
        CString strFolder = strFilePath.Mid(0, strFilePath.ReverseFind('\\'));
        m_folderDropList.AddString(strFolder);
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

void CMiiGameDlg::Progress(int status, int total) {
    float fRatio = (float)status / (float)total;
    CStringA strMsg;
    strMsg.Format("%.2f\n", fRatio);
    TRACE(strMsg.GetString());
}

void __stdcall progress(int status, int total) {
    CMiiGameDlg::Progress(status, total);
}

//UINT CMiiGameDlg::UploadImageThread(LPVOID pParam) {
//	CMiiGameDlg* pThis = (CMiiGameDlg*)pParam;
//	char szNewName[512] = {0};
//	pThis->m_driveControl.UploadImageToWBFS((char*)pThis->m_vecISOEntries.at(j).m_strImagePath.GetString(),
//		(progress_callback_t)progress, ONLY_GAME_PARTITION, false, szNewName);
//}

void CMiiGameDlg::OnBnClickedMoveLeftBtn()
{
    // Add disk to drive (from your HD to WBFS)
    vector<CString> vecSelectName;
    POSITION pos = m_isoList.GetFirstSelectedItemPosition();
    if(pos == NULL) return;
    while(pos != NULL) {
        int nItem = m_isoList.GetNextSelectedItem(pos);
        CString strItemText = m_isoList.GetItemText(nItem, 0);
        vecSelectName.push_back(strItemText);
    }
    if(this->OpenDrive() != true) return;
    for(int i = 0; i < vecSelectName.size(); ++i) {
        CStringA strSelectedName;
        W2MB(vecSelectName.at(i), strSelectedName);
        for(int j = 0; j < m_vecISOEntries.size(); ++j) {
            if(m_vecISOEntries.at(j).m_strDiskName == strSelectedName) {
                char szNewName[512] = {0};
                int nCode = m_driveControl.UploadImageToWBFS((char*)m_vecISOEntries.at(j).m_strImagePath.GetString(), (progress_callback_t)progress, ONLY_GAME_PARTITION, false, szNewName);
                if(nCode == 0) {
                    AfxMessageBox(_T("Upload to WBFS successfully..."), MB_OK);
                } else {
                    CString strMsg;
                    switch(nCode) {
case -1:
    strMsg = _T("Partition wasn't loaded previously");
    break;
case -2:
    strMsg = _T("Error occured while attempting to read file");
    break;
case -3:
    strMsg = _T("Disc already exists on WBFS drive");
    break;
                    }
                    AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK);
                }
            }
        }
    }
    CloseDrive();
    OnBnClickedLoadBtn();
}

void CMiiGameDlg::OnBnClickedMoveRightBtn()
{
    // TODO: Add your control notification handler code here
}

void CMiiGameDlg::OnBnClickedFormatBtn()
{
    int nRet = AfxMessageBox(_T("Are you sure to format this driver?"), MB_YESNO);
    if(nRet == IDNO) return;

    CString strDriveLetter;
    m_driveCombo.GetWindowText(strDriveLetter);
    CStringA straDriveLetter;
    straDriveLetter.Format("%S", strDriveLetter.GetString());
    straDriveLetter.Replace(":", "");
    CloseDrive();
    if(!FormatDrive((char*)straDriveLetter.GetString())) {
        AfxMessageBox(_T("Format failed..."));
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
