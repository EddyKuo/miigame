// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MiiGame.h"
#include "ProgressDlg.h"


// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DESCRIPTION, m_descript);
    DDX_Control(pDX, IDC_COMPLETE_ITEM, m_complete);
    DDX_Control(pDX, IDC_PERCENTAGE, m_percentage);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
END_MESSAGE_MAP()


// CProgressDlg message handlers

void CProgressDlg::Event(const TSTRING& strEvent,long nParam) {
    if(strEvent == _T("Upload Complete") || strEvent == _T("Download Complete")) {
        // add 1 to complete item
    } else if(strEvent == _T("Upload Progress") || strEvent == _T("Download Progress")) {
        // update the progress bar
    } else if(strEvent == _T("Upload All Complete") || strEvent == _T("Download All Complete")) {
        // close this dialog
    }

}
BOOL CProgressDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_descript.SetBkColor(RGB(0, 200, 0));
    m_complete.SetBkColor(RGB(0, 200, 0));
    m_percentage.SetBkColor(RGB(0, 200, 0));

    return TRUE;  
}
