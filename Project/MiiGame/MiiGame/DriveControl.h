#pragma once
#include <vector>
#include "ImageEntry.h"

using namespace std;

class CDriveControl
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
	int UploadImageToWBFS(char* strImagePath, progress_callback_t pFnCallback, partition_selector_t selector, bool copy1to1, char *newName);
public:
    vector<CString> m_vecDrivers;

};
