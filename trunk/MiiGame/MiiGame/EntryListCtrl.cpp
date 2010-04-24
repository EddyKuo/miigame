// EntryListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MiiGame.h"
#include "EntryListCtrl.h"


// CEntryListCtrl

IMPLEMENT_DYNAMIC(CEntryListCtrl, CListCtrl)

CEntryListCtrl::CEntryListCtrl()
{
    DefineNotify(EVENT_ZERO_LENGTH_EDITION);
    DefineNotify(EVENT_CHANGE_DISK_NAME);
    DefineNotify(EVENT_DROP_FILES);
}

CEntryListCtrl::~CEntryListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEntryListCtrl, CListCtrl)
    ON_WM_KEYDOWN()
    ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, &CEntryListCtrl::OnLvnBeginlabeledit)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CEntryListCtrl::OnLvnEndlabeledit)
    ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CEntryListCtrl message handlers

void CEntryListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    switch( nChar )
    {
    case VK_F2:
        {
            ASSERT(GetStyle() & LVS_EDITLABELS);

            // don't do an Edit Label when the multiple Items are selected
            if(GetSelectedCount( ) == 1 )
            {
                UINT nListSelectedItem = GetSelectedItem();
                VERIFY(EditLabel( nListSelectedItem ) != NULL);
            }
            else
                CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
        }
        break;

    default:
        CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
        break;
    }
}

UINT CEntryListCtrl::GetSelectedItem()
{
    // this Function Valid Only when a Single Item Is Selected
    ASSERT(GetSelectedCount( ) == 1 );

    UINT nNoOfItems = GetItemCount( );
    UINT nListItem = 0;
    for(nListItem = 0; nListItem < nNoOfItems; nListItem++)
        if(GetItemState(nListItem, LVIS_SELECTED))
            break;

    ASSERT(nListItem < nNoOfItems);
    return nListItem;
}

void CEntryListCtrl::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    GetEditControl()->LimitText(512);
    *pResult = 0;
}

void CEntryListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    Fire(EVENT_CHANGE_DISK_NAME, (long)pDispInfo);
    *pResult = 1;
}

BOOL CEntryListCtrl::PreTranslateMessage(MSG* pMsg)
{
#define VK_C		67
#define VK_V		86
#define VK_X		88
#define VK_Z		90
    // TODO: Add your specialized code here and/or call the base class
    if ( pMsg->message == WM_KEYDOWN )
    {
        CEdit* edit = GetEditControl();
        if(edit)
        {
            if( GetKeyState( VK_CONTROL ) && pMsg->wParam == VK_C )
            {
                edit->Copy();
                return TRUE;
            }
            if( GetKeyState( VK_CONTROL ) && pMsg->wParam == VK_V )
            {
                edit->Paste();
                return TRUE;
            }
            if( GetKeyState( VK_CONTROL ) && pMsg->wParam == VK_X )
            {
                edit->Cut();
                return TRUE;
            }
            if( GetKeyState( VK_CONTROL ) && pMsg->wParam == VK_Z )
            {
                edit->Undo();
                return TRUE;
            }
            if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CONTROL || pMsg->wParam == VK_INSERT || pMsg->wParam == VK_SHIFT )
            {
                edit->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
                return TRUE;
            }
        }
    }
    return CListCtrl::PreTranslateMessage(pMsg);
}

void CEntryListCtrl::Event(const TSTRING& strEvent,long nParam) {

}
void CEntryListCtrl::OnDropFiles(HDROP hDropInfo)
{
    // TODO: Add your message handler code here and/or call default
    int nFileCount = DragQueryFileA(hDropInfo,0xFFFFFFFF, NULL, 0);
    vector<CStringA> vecFiles;
    for(int i = 0; i < nFileCount; ++i) {
        int nNameLength = DragQueryFileA(hDropInfo, i, NULL, 0);
        char* szFileName = new char[nNameLength + 2];
        memset(szFileName, 0, nNameLength + 2);
        if(DragQueryFileA(hDropInfo, i, szFileName, nNameLength + 2) != 0) {
            vecFiles.push_back(szFileName);
        }
        delete szFileName;
    }
    TRACE("Files dropped...");
    Fire(EVENT_DROP_FILES, (long)&vecFiles);
    __super::OnDropFiles(hDropInfo);
}
