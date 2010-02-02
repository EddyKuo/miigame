// Localization.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Localization.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;
using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}

inline CString GetCurrentPath() {
    TCHAR szPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szPath, MAX_PATH);
    CString temp = szPath;
    return temp.Mid(0, temp.ReverseFind('\\') + 1);
}

CLocalization::CLocalization() : m_currentLCID(GetThreadLocale()) {
    FindLanguageFile();
    m_strLCID.Format(_T("0x%.4X"), m_currentLCID);
}
CLocalization::~CLocalization() {

}

void CLocalization::FindLanguageFile()
{
    CString strCurrentPath = GetCurrentPath();
    strCurrentPath += _T("language\\");
    CString strFindFile = strCurrentPath + _T("*.txt");

    CFileFind finder;
    BOOL bWorking = finder.FindFile(strFindFile.GetString());
    while(bWorking) {
        bWorking = finder.FindNextFile();
        CString strFullPathName = finder.GetFilePath();
        GetLCIDFromFile(strFullPathName);
    }
}

void CLocalization::GetLCIDFromFile(const CString& fileName) {
    CStdioFileEx file;
    file.Open(fileName.GetString(), CFile::modeRead);
    CString strTmp, currentSection;
    map<CString, CString> mapIDString;
    LCID id = 0;
    while(file.ReadString(strTmp)) {
        if(strTmp.GetLength() >= 2 && strTmp.GetAt(0) == _T('/') && strTmp.GetAt(0) == _T('/')) continue;
        if(strTmp.GetLength() == 0) continue;
        if(strTmp == _T("[LCID]") || strTmp == _T("[IDS]")) {
            currentSection = strTmp;
            continue;
        }
        // process LCID
        if(currentSection == _T("[LCID]")) {
            CString strLCID = strTmp.Mid(strTmp.Find(_T('='))+1);
            id = _tcstol(strLCID.GetString(), NULL, 16);
        }
        // process IDS
        else if(currentSection == _T("[IDS]")) {
            CString strIDS = strTmp.Mid(0, strTmp.Find(_T('=')));
            CString strString = strTmp.Mid(strTmp.Find(_T('='))+1);
            strString.Replace(_T("\\r\\n"), _T("\r\n"));
            strString.Trim();
            mapIDString[strIDS] = strString;
        }
    }
    m_mapLCIDToIDString[id] = mapIDString;
}

CString CLocalization::GetIDString(const CString& IDS) {
    if(IDS.Find(_T("IDS")) == -1) return IDS;
    map<CString, CString> mapIDString = m_mapLCIDToIDString.find(m_currentLCID) != 
        m_mapLCIDToIDString.end() ? 
        m_mapLCIDToIDString[m_currentLCID] : m_mapLCIDToIDString[0x0409];
    return (mapIDString.find(IDS) != mapIDString.end() ? mapIDString[IDS] : m_mapLCIDToIDString[0x0409][IDS]);
}

LOCALIZATION_API CLocalization* GetLocalizationInstance() {
    if(g_localization == NULL) {
        g_localization = new CLocalization();
    }
    return g_localization;
}

LOCALIZATION_API void ReleaseLocalizationInstance() {
    if(g_localization != NULL)
        delete g_localization;
    g_localization = NULL;
}