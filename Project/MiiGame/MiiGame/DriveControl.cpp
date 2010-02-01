#include "StdAfx.h"
#include "DriveControl.h"

CDriveControl::CDriveControl()
{
    UpdateDrivers();
}

CDriveControl::~CDriveControl()
{
}

vector<CString> CDriveControl::GetDrivers() {
    return m_vecDrivers;
}

void CDriveControl::UpdateDrivers() {
    m_vecDrivers.clear();
    TCHAR szDrivers[512] = {0};
    DWORD nLength = ::GetLogicalDriveStrings(512, szDrivers);
    for(int i = 0; i < nLength; i = i + 4) {
        CString strDriverLetter = &(szDrivers[i]);
        strDriverLetter.Replace(_T("\\"), _T(""));
        m_vecDrivers.push_back(strDriverLetter);
    }
}