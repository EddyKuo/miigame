#pragma once


// CEntryListCtrl

class CEntryListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CEntryListCtrl)

public:
	CEntryListCtrl();
	virtual ~CEntryListCtrl();
private:
    UINT GetSelectedItem();
protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};


