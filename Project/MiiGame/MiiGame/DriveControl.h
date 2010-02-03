#pragma once
#include <vector>
#include "ImageEntry.h"
#include "NotifyBase.h"

static TSTRING EVENT_UPLOAD_COMPLETE = _T("Upload Complete");
static TSTRING EVENT_UPLOAD_PROGRESS = _T("Upload Progress");
static TSTRING EVENT_DOWNLOAD_COMPLETE = _T("Download Complete");
static TSTRING EVENT_DOWNLOAD_PROGRESS = _T("Download Progress");

using namespace std;

class CDriveControl : public CINotifyBase
{
public:
    CDriveControl();
    ~CDriveControl();
public:
    vector<CString> GetDrivers();
    void UpdateDrivers();
    bool OpenDrive(CStringA strDriveLetter);
    vector<CImageEntry> GetDiskFromDrive(CStringA strDriveLetter);
    CImageEntry GetImageFromHD(CString& strFilePath);
    int UploadImageToWBFS(CStringA strImagePath, partition_selector_t selector, bool copy1to1, char *newName);
    static UINT UploadThread(LPVOID nParam);

    int ExtractDiskToHD(CStringA strGameID, CStringA newFileName);
    static UINT ExtractThread(LPVOID nParam);

    void Event(const TSTRING& strEvent,long nParam);
public:
    vector<CString> m_vecDrivers;

};
