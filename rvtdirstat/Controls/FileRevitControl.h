#pragma once

class CFileRevitControl final : public CTreeListControl
{
public:
    CFileRevitControl();
    ~CFileRevitControl() override { m_singleton = nullptr; }
    bool GetAscendingDefault(int column) override;
    static CFileRevitControl* Get() { return m_singleton; }
    void ProcessRevitFiles(CItem* item);
    void SortItems();
    void RemoveItem(CItem* items);

protected:
    // Custom comparator to keep the list organized by size (largest first)
    static constexpr auto CompareBySize = [](const CItem* lhs, const CItem* rhs)
    {
        return lhs->GetSizeLogical() > rhs->GetSizeLogical();
    };

    static CFileRevitControl* m_singleton;
    SingleConsumerQueue<CItem*> m_queuedSet;
    std::vector<CItem*> m_sizeMap;
    ULONGLONG m_revitNMinSize = 0;
    bool m_needsResort = true;
    std::unordered_map<CItem*, CItemRevit*> m_itemTracker;
    size_t m_previousRevitN = 0;

    void OnItemDoubleClick(int i) override;

    DECLARE_MESSAGE_MAP()
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnDeleteAllItems(NMHDR* pNMHDR, LRESULT* pResult);
};
