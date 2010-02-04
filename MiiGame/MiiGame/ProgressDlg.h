#pragma once
#include "NotifyBase.h"
#include "../UIComponent/ColorStatic.h"
#include "m:\miigame\uicomponent\colorstatic.h"
#include "m:\miigame\uicomponent\colorstatic.h"
#include "m:\miigame\uicomponent\colorstatic.h"
// CProgressDlg dialog

class CProgressDlg : public CDialog, public INotify
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();
    virtual void Event(const TSTRING& strEvent,long nParam);

// Dialog Data
	enum { IDD = IDD_PROG_DLG };
private:
    CColorStatic m_descript;
    CColorStatic m_complete;
    CColorStatic m_percentage;
protected:
    DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
	

    
};
