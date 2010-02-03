#pragma once

#include "../libwbfsNETwrapper/WbfsInterm.h"

class CImageEntry
{
public:

    CStringA m_strDiskName;     // disk Name
    CStringA m_strImagePath;    // full path for the UI
    CStringA m_strDiskID;       // Disk ID 6 codes
    float m_fDiskSize;          // disk size
    RegionCode m_regionCode;    // region code

    CImageEntry(CStringA strDiskName, CStringA strDiskID, float fDiskSize, RegionCode regionCode, CStringA strImagePath = "")
    {
        m_strDiskName = strDiskName;
        m_strDiskID = strDiskID;
        m_fDiskSize = fDiskSize;
        m_regionCode = regionCode;
        m_strImagePath = strImagePath;
    }

    ~CImageEntry(void)
    {
    }
};
