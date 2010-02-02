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
public:
    vector<CString> m_vecDrivers;

};
