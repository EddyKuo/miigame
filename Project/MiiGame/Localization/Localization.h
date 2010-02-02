// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LOCALIZATION_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LOCALIZATION_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LOCALIZATION_EXPORTS
#define LOCALIZATION_API __declspec(dllexport)
#else
#define LOCALIZATION_API __declspec(dllimport)
#endif

#include <map>

using namespace std;

// This class is exported from the Localization.dll
class LOCALIZATION_API CLocalization {
public:
	CLocalization();
    virtual ~CLocalization();
public:
    CString GetIDString(const CString& IDS);
private:
    void FindLanguageFile();
    void GetLCIDFromFile(const CString& fileName);
private:
    map<LCID, CString> m_mapLanguageFile;
    map<LCID, map<CString, CString>> m_mapLCIDToIDString;
    LCID m_currentLCID;
    CString m_strLCID;
};

//extern LOCALIZATION_API int nLocalization;

LOCALIZATION_API CLocalization* g_localization;

LOCALIZATION_API int fnLocalization(void);
LOCALIZATION_API CLocalization* GetLocalizationInstance();
LOCALIZATION_API void ReleaseLocalizationInstance();
