// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiiGame.h"
#include "ProgressDlg.h"
#include "../Localization/Localization.h"

// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

static map<int, CString> g_mapAnimate;

CProgressDlg::CProgressDlg(CString strDrsc, CString strComp, CString strPencetage, CWnd* pParent)
	: CDialog(CProgressDlg::IDD, pParent), m_desc(strDrsc), m_comp(strComp), m_perc(strPencetage), m_bInitialized(false)
{
    g_mapAnimate[0] = _T("|");
    g_mapAnimate[1] = _T("/");
    g_mapAnimate[2] = _T("--");
    g_mapAnimate[3] = _T("\\");
}

CProgressDlg::~CProgressDlg()
{
    ReleaseLocalizationInstance();
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DESCRIPTION, m_descript);
    DDX_Control(pDX, IDC_COMPLETE_ITEM, m_complete);
    DDX_Control(pDX, IDC_PERCENTAGE, m_percentage);
    DDX_Control(pDX, IDC_PROGRESS1, m_progressbar);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressDlg message handlers

void CProgressDlg::Event(const TSTRING& strEvent,long nParam) {
    if(!m_bInitialized) return;
    if(strEvent == _T("Upload Complete") || strEvent == _T("Download Complete")) {
        // add 1 to complete item
        m_nCompleteItems++;
        CString strComplete;
        strComplete.Format(_T(" %d/%d"), m_nCompleteItems, m_nTotalItems);
        m_complete.SetWindowText(GetLocalizationInstance()->GetIDString(_T("IDS_Total_Complete")) + strComplete);
    } else if(strEvent == _T("Upload Progress") || strEvent == _T("Download Progress")) {
        // update the progress bar
        int nStatus = nParam >> 16, nTotal = nParam & 0xFFFF;
        UpdateProgressbar(nStatus, nTotal);
    } else if(strEvent == _T("Upload All Complete") || strEvent == _T("Download All Complete")) {
        KillTimer(1);
        SendMessage(WM_CLOSE);
    }
}

BOOL CProgressDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    //m_descript.SetBkColor(RGB(0, 200, 0));
    //m_complete.SetBkColor(RGB(0, 200, 0));
    //m_percentage.SetBkColor(RGB(0, 200, 0));

    m_descript.SetWindowText(m_desc);
    m_complete.SetWindowText(m_comp);
    m_percentage.SetWindowText(m_perc);
    GetDlgItem(IDC_PROG_ANIMATE)->SetWindowText(_T(""));
    m_bInitialized = true;

    //GetDlgItem(IDOK)->EnableWindow(FALSE);
    //GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
    SetTimer(1, 500, NULL);

    return TRUE;  
}

void CProgressDlg::UpdateProgressbar(int current, int total) {
    if(!m_bInitialized) return;
    int nPercentage = (int) (((float)current) / ((float)total) * 100);
    CString strPercentage;
    strPercentage.Format(_T(" %.2f%%"), ((float)current / (float)total)*100);
    strPercentage = CString(GetLocalizationInstance()->GetIDString(_T("IDS_Progress_Complete_Percentage"))) + strPercentage;
    m_percentage.SetWindowText(strPercentage);
    m_progressbar.SetPos(nPercentage);
    m_progressbar.UpdateWindow();
}

void CProgressDlg::SetItemInformation(int nCompelete, int nTotal) {
    m_nCompleteItems = nCompelete;
    m_nTotalItems = nTotal;
}

void CProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
    static int i = 0;
    switch(nIDEvent) {
        case 1:
            GetDlgItem(IDC_PROG_ANIMATE)->SetWindowText(g_mapAnimate[(i++)%4]);
            break;
    }

    __super::OnTimer(nIDEvent);
}
