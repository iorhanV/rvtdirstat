
#pragma once

#include "pch.h"

// Columns
using ITEMREVITCOLUMNS = enum : std::uint8_t
{
    COL_ITEMREVIT_NAME,
    COL_ITEMREVIT_SIZE_PHYSICAL,
    COL_ITEMREVIT_SIZE_LOGICAL,
    COL_ITEMREVIT_LAST_CHANGE
};

class CItemRevit final : public CTreeListItem
{
    std::shared_mutex m_protect;
    std::vector<CItemRevit*> m_children;
    CItem* m_item = nullptr;

public:
    CItemRevit(const CItemRevit&) = delete;
    CItemRevit(CItemRevit&&) = delete;
    CItemRevit& operator=(const CItemRevit&) = delete;
    CItemRevit& operator=(CItemRevit&&) = delete;
    CItemRevit() = default;
    CItemRevit(CItem* item);
    ~CItemRevit() override;

    // Translation map for leveraging Item routines
    static const std::unordered_map<uint8_t, uint8_t> s_columnMap;

    // CTreeListItem Interface
    bool DrawSubItem(int subitem, CDC* pdc, CRect rc, UINT state, int* width, int* focusLeft) override;
    std::wstring GetText(int subitem) const override;
    int CompareSibling(const CTreeListItem* tlib, int subitem) const override;
    int GetTreeListChildCount() const override;
    CTreeListItem* GetTreeListChild(int i) const override;
    COLORREF GetItemTextColor() const;
    HICON GetIcon() override;
    CItem* GetLinkedItem() override { return m_item; }

    void AddRevitItemChild(CItemRevit* child);
    void RemoveRevitItemChild(CItemRevit* child);
    void RemoveRevitItemResults();
};
