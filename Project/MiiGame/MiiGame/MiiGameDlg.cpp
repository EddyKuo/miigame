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
}

BEGIN_MESSAGE_MAP(CMiiGameDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_LOAD_BTN, &CMiiGameDlg::OnBnClickedLoadBtn)
    ON_BN_CLICKED(IDC_UPDATE_DRIVE_BTN, &CMiiGameDlg::OnBnClickedUpdateDriveBtn)
    ON_BN_CLICKED(IDC_OPEN_FILE_BTN, &CMiiGameDlg::OnBnClickedOpenFileBtn)
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

	// TODO: Add extra initialization here

    OnBnClickedUpdateDriveBtn();

    m_diskList.InsertColumn(0, _T("Game Title"), 0, 170);
    m_diskList.InsertColumn(1, _T("Game ID"), 0, 60);
    m_diskList.InsertColumn(2, _T("Image Size"), 0, 60);
    //m_diskList.InsertColumn(3, _T("Region Code"), 0, 50);
    m_diskList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


    m_isoList.InsertColumn(0, _T("Game Title"), 0, 170);
    m_isoList.InsertColumn(2, _T("Image Size"), 0, 60);
    m_isoList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
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
    if(!::OpenDrive((char*)straDriveLetter.GetString())) {
        AfxMessageBox(_T("Open this drive failed..."));
        return false;
    }
    return true;
}

void CMiiGameDlg::GetDisks() {
    m_vecImageEntries.clear();
    int nDiskCount = GetDiscCount();
    for(int i = 0; i < nDiskCount; ++i) {
        char szDiskName[512] = {0};
        char szDiskID[128] = {0};
        float fDiskSize = 0;
        RegionCode regionCode = NOREGION;
        int nRetCode = GetDiscInfoEx(i, szDiskID, &fDiskSize, szDiskName, & regionCode);
        if(nRetCode < 0) {
            CString strMessage;
            strMessage.Format(_T("Get Disk Error ==> #%d"), i);
            AfxMessageBox(strMessage);
        } else {
            CImageEntry entry(szDiskName, szDiskID, fDiskSize, regionCode);
            m_vecImageEntries.push_back(entry);
        }
    }
}

void CMiiGameDlg::SetInformationToList()
{
    m_diskList.SetRedraw(FALSE);
    m_diskList.DeleteAllItems();
    for(int i = 0; i < m_vecImageEntries.size(); ++i) {
        CString strDiskName, strDiskID;
        strDiskName.Format(_T("%S"), m_vecImageEntries.at(i).m_strDiskName.GetString());
        m_diskList.InsertItem(i, strDiskName);
        strDiskID.Format(_T("%S"), m_vecImageEntries.at(i).m_strDiskID.GetString());
        m_diskList.SetItemText(i, 1, strDiskID);
        CString strSize;
        strSize.Format(_T("%.2f GB"), m_vecImageEntries.at(i).m_fDiskSize);
        m_diskList.SetItemText(i, 2, strSize);
        //m_diskList.SetItemText(i, 3, m_vecImageEntries.at(i).m_strDiskID.GetString());
    }
    m_diskList.SetRedraw(TRUE);
}
void CMiiGameDlg::OnBnClickedOpenFileBtn()
{
#define GB (1024 * 1024 * 1024.)
#define MB (1024 * 1024)
    CString strFilter = _T("rar|*.rar|zip|*.zip|iso|*.iso||");
    CFileDialog fd(TRUE, 0, 0, OFN_ALLOWMULTISELECT, strFilter, this);
    if(fd.DoModal() == IDOK) {
        POSITION pos = fd.GetStartPosition();
        int i = 0;
        while(pos != NULL) {
            CString strFilePath = fd.GetNextPathName(pos);
            CFile f;
            f.Open(strFilePath, CFile::modeRead);
            CString strFileName = strFilePath.Mid(strFilePath.ReverseFind('\\')+1);

            CStringA straFileName, straFilePath;
            W2MB(strFileName, straFileName);
            W2MB(strFilePath, straFilePath);

            long nBytes = f.GetLength();
            float fSize = nBytes / GB;
            CImageEntry entry(straFileName, "", fSize, NOREGION, straFilePath);
            m_vecISOEntries.push_back(entry);
            m_isoList.InsertItem(i, strFileName);
            CString strSize;
            strSize.Format(_T("%.2fGB"), fSize);
            m_isoList.SetItemText(i, 1, strSize);
        }
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