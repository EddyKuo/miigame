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

bool CDriveControl::OpenDrive(CStringA strDriveLetter) {
    if(!::OpenDrive((char*)strDriveLetter.GetString())) {
        AfxMessageBox(_T("Open this drive failed..."));
        return false;
    }
    return true;
}

vector<CImageEntry> CDriveControl::GetDiskFromDrive(CStringA strDriveLetter) {
    vector<CImageEntry> entries;
    int nDiskCount = GetDiscCount();
    if(nDiskCount == -1) {
        OpenDrive(strDriveLetter);
    }
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
            entries.push_back(entry);
        }
    }
    return entries;
}
void W2MB(const CString& a, CStringA& b) {
    DWORD dwNum  = WideCharToMultiByte(CP_OEMCP, 0, a.GetString(), -1, 0, 0, 0, 0);
    char *psText = new char[dwNum+2];
    memset(psText, 0, dwNum+2);
    WideCharToMultiByte (CP_OEMCP , 0, a.GetString(), -1, psText, dwNum, 0, 0);
    b = psText;
    delete [] psText;
}

CImageEntry CDriveControl::GetImageFromHD(CString& strFilePath) {
#define GB (1024 * 1024 * 1024.)
#define MB (1024 * 1024)
    CFile f;
    f.Open(strFilePath, CFile::modeRead);
    CString strFileName = strFilePath.Mid(strFilePath.ReverseFind('\\')+1);
    CStringA straFileName, straFilePath;
    W2MB(strFileName, straFileName);
    W2MB(strFilePath, straFilePath);
    ULONGLONG nBytes = f.GetLength();
    float fSize = nBytes / GB;
    CImageEntry entry(straFileName, "", fSize, NOREGION, straFilePath);
    return entry;
}

int CDriveControl::UploadImageToWBFS(char* strImagePath, progress_callback_t pFnCallback, partition_selector_t selector, bool copy1to1, char *newName) {
	return AddDiscToDrive(strImagePath, (progress_callback_t)pFnCallback, selector, false, newName);
}
