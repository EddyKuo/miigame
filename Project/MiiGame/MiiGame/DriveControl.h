#pragma once
#include <vector>


using namespace std;

class CDriveControl
{
public:
    CDriveControl();
    ~CDriveControl();
public:
    vector<CString> GetDrivers();
    void UpdateDrivers();
public:
    vector<CString> m_vecDrivers;

};
