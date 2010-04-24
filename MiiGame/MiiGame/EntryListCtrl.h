#pragma once

#include "NotifyBase.h"
// CEntryListCtrl

static TSTRING EVENT_ZERO_LENGTH_EDITION = _T("Zero Length");
static TSTRING EVENT_CHANGE_DISK_NAME = _T("Change Name");
static TSTRING EVENT_DROP_FILES = _T("Drop Files");

class CEntryListCtrl : public CListCtrl, public CINotifyBase
{
    DECLARE_DYNAMIC(CEntryListCtrl)

public:
    CEntryListCtrl();
    virtual ~CEntryListCtrl();
    virtual void Event(const TSTRING& strEvent,long nParam);
private:
    UINT GetSelectedItem();
protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
    afx_msg void OnDropFiles(HDROP hDropInfo);
};


