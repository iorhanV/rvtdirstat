#pragma once

#include "pch.h"

#include "FileRevitControl.h"

class CFileRevitView final : public CView
{
protected:
    CFileRevitView(); // Created by MFC only
    DECLARE_DYNCREATE(CFileRevitView)

    ~CFileRevitView() override = default;
    void SysColorChanged();

protected:
    void OnDraw(CDC* pDC) override;
    void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;

    CFileRevitControl m_control;

    DECLARE_MESSAGE_MAP()
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnLvnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnUpdatePopupToggle(CCmdUI* pCmdUI);
    afx_msg void OnPopupToggle();
};


