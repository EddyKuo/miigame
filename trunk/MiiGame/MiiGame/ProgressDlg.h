#pragma once
#include "NotifyBase.h"
#include "../UIComponent/ColorStatic.h"
#include "afxcmn.h"
// CProgressDlg dialog

class CProgressDlg : public CDialog, public INotify
{
	DECLARE_DYNAMIC(CProgressDlg)
    enum { IDD = IDD_PROG_DLG };
public:
	CProgressDlg(CString strDrsc, CString strComp, CString strPencetage, CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();
    virtual void Event(const TSTRING& strEvent,long nParam);
    void SetItemInformation(int nCompelete, int nTotal);
private:
    void UpdateProgressbar(int current, int total);
// Dialog Data
private:
    int m_nTotalItems;
    int m_nCompleteItems;
    bool m_bInitialized;

    CString m_desc;
    CString m_comp;
    CString m_perc;
    CColorStatic m_descript;
    CColorStatic m_complete;
    CColorStatic m_percentage;
    CProgressCtrl m_progressbar;
protected:
    DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
