#pragma once
#include <vector>
#include "ImageEntry.h"
#include "NotifyBase.h"

static TSTRING EVENT_UPLOAD_ALL_COMPLETE = _T("Upload All Complete");
static TSTRING EVENT_UPLOAD_COMPLETE = _T("Upload Complete"); // 1 complete or error
static TSTRING EVENT_UPLOAD_FAILED = _T("Upload Failed");
static TSTRING EVENT_UPLOAD_PROGRESS = _T("Upload Progress");

static TSTRING EVENT_DOWNLOAD_ALL_COMPLETE = _T("Download All Complete");
static TSTRING EVENT_DOWNLOAD_COMPLETE = _T("Download Complete"); // 1 complete or error
static TSTRING EVENT_DOWNLOAD_FAILED = _T("Download Failed");
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

    // used for upload image
    int UploadImageToWBFS(vector<CString>* pvecSelectName, vector<CImageEntry>& vecImage);
    static UINT UploadThread(LPVOID nParam);

    // used for download image
    int ExtractDiskToHD(vector<CString>* pvecSelectName, vector<CImageEntry>& vecImage);
    static UINT ExtractThread(LPVOID nParam);

    bool GetFolder(TSTRING& folderpath, const TCHAR* szCaption = NULL, HWND hOwner = NULL);

    int RenameDisk(CStringA strDiskID, CStringA strNewName);

    int GetDriveStatus(unsigned int& blocks, float& total, float& used, float& free);
    void Event(const TSTRING& strEvent,long nParam);
public:
    vector<CString> m_vecDrivers;

};
