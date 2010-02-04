#include "StdAfx.h"
#include "DriveControl.h"
#include "../Localization/Localization.h"


static CDriveControl* g_pThis;
CDriveControl::CDriveControl()
{
    UpdateDrivers();
    DefineNotify(EVENT_UPLOAD_COMPLETE);
    DefineNotify(EVENT_UPLOAD_PROGRESS);
    DefineNotify(EVENT_UPLOAD_ALL_COMPLETE);
    DefineNotify(EVENT_UPLOAD_FAILED);
    DefineNotify(EVENT_DOWNLOAD_COMPLETE);
    DefineNotify(EVENT_DOWNLOAD_PROGRESS);
    DefineNotify(EVENT_DOWNLOAD_ALL_COMPLETE);
    DefineNotify(EVENT_DOWNLOAD_FAILED);

    g_pThis = this;
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
        AfxMessageBox(GetLocalizationInstance()->GetIDString(_T("IDS_Open_this_drive_failed")));
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
            CString strMessage = GetLocalizationInstance()->GetIDString(_T("IDS_Get_Disk_Error"));
            CString strNum;
            strNum.Format(_T(" ==> #%d"), i);
            AfxMessageBox(strMessage + strNum);
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
    WideCharToMultiByte(CP_OEMCP , 0, a.GetString(), -1, psText, dwNum, 0, 0);
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

typedef struct upload_param {
    CDriveControl* pThis;
    vector<CString>* pvecSelectName;
    vector<CImageEntry>* pvecImage;
} UL_Param;

void __stdcall UploadProgress(int status, int total) {
    long nParam = (status << 16) | total;
    g_pThis->Fire(EVENT_UPLOAD_PROGRESS, nParam);
}

int CDriveControl::UploadImageToWBFS(vector<CString>* pvecSelectName, vector<CImageEntry>& vecImage) {
    UL_Param *p = new UL_Param;
    p->pThis = this;
    p->pvecSelectName = pvecSelectName;
    p->pvecImage = &vecImage;
    CWinThread* thread = AfxBeginThread(CDriveControl::UploadThread, p);
    return 0;
}

UINT CDriveControl::UploadThread(LPVOID nParam) {
    UL_Param *p = (UL_Param*)nParam;
    char szNewName[512] = {0};
    for(int i = 0; i < p->pvecSelectName->size(); ++i) {
        CStringA strSelectedName;
        W2MB(p->pvecSelectName->at(i), strSelectedName);
        for(int j = 0; j < p->pvecImage->size(); ++j) {
            if(p->pvecImage->at(j).m_strDiskName == strSelectedName) {
                char szNewName[512] = {0};
                int nCode = AddDiscToDrive((char*)p->pvecImage->at(j).m_strImagePath.GetString(), UploadProgress, ONLY_GAME_PARTITION, false, szNewName);
                p->pThis->Fire(EVENT_UPLOAD_COMPLETE, nCode);
            }
        }
    }
    p->pThis->Fire(EVENT_UPLOAD_ALL_COMPLETE);
    delete p->pvecSelectName;
    delete p;
    return 0;
}


typedef struct download_param {
    CDriveControl* pThis;
    vector<CString>* pvecSelectName;
    vector<CImageEntry>* pvecImage;
} DL_Param;

void __stdcall DownloadProgress(int status, int total) {
    long nParam = (status << 16) | total;
    g_pThis->Fire(EVENT_DOWNLOAD_PROGRESS, nParam);
}

int CDriveControl::ExtractDiskToHD(vector<CString>* pvecSelectName, vector<CImageEntry>& vecImage) {
    DL_Param* p = new DL_Param;
    p->pThis = this;
    p->pvecSelectName = pvecSelectName;
    p->pvecImage = &vecImage;
    CWinThread* thread = AfxBeginThread(CDriveControl::ExtractThread, p);
    return 0;
}

UINT CDriveControl::ExtractThread(LPVOID nParam) {
    DL_Param* p = (DL_Param*)nParam;
    TSTRING strPath;
    p->pThis->GetFolder(strPath, _T("Mii Game Manager"));
    if(strPath == _T("")) {
        //AfxMessageBox(_T("Please select a folder to download"), MB_ICONINFORMATION, MB_OK);
        p->pThis->Fire(EVENT_DOWNLOAD_FAILED);
        delete p->pvecSelectName;
        delete p;
        return 0;
    }
    for(int i = 0; i < p->pvecSelectName->size(); ++i) {
        CStringA strSelectedName;
        W2MB(p->pvecSelectName->at(i), strSelectedName);
        for(int j = 0; j < p->pvecImage->size(); ++j) {
            if(p->pvecImage->at(j).m_strDiskName == strSelectedName) {
                TSTRING strISOLocation = strPath + _T("\\") + TSTRING(p->pvecSelectName->at(i)) + _T(".iso");
                CStringA straNewName;
                W2MB(strISOLocation.c_str(), straNewName);
                int nCode = ExtractDiscFromDrive((char*)p->pvecImage->at(j).m_strDiskID.GetString(), DownloadProgress, (char*)straNewName.GetString());
                p->pThis->Fire(EVENT_DOWNLOAD_COMPLETE, nCode);
            }
        }
    }
    p->pThis->Fire(EVENT_DOWNLOAD_ALL_COMPLETE);
    delete p->pvecSelectName;
    delete p;
    return 0;
}

bool CDriveControl::GetFolder(TSTRING& folderpath, const TCHAR* szCaption, HWND hOwner) {
    bool retVal = false;
    BROWSEINFO bi;
    memset(&bi, 0, sizeof(bi));
    bi.ulFlags   = BIF_USENEWUI;
    bi.hwndOwner = hOwner;
    bi.lpszTitle = szCaption;
    ::OleInitialize(NULL);
    LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);
    if(pIDL != NULL)
    {
        TCHAR buffer[_MAX_PATH] = {'\0'};
        if(::SHGetPathFromIDList(pIDL, buffer) != 0)
        {
            folderpath = buffer;
            retVal = true;
        }
        CoTaskMemFree(pIDL);
    }
    ::OleUninitialize();
    return retVal;
}

void CDriveControl::Event(const TSTRING& strEvent,long nParam) {

}