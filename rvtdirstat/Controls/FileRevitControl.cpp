
#include "pch.h"
#include "ItemRevit.h"
#include "FileRevitControl.h"

CFileRevitControl::CFileRevitControl() : CTreeListControl(COptions::TopViewColumnOrder.Ptr(), COptions::TopViewColumnWidths.Ptr())
{
    m_singleton = this;
}

bool CFileRevitControl::GetAscendingDefault(const int column)
{
    return column == COL_ITEMREVIT_SIZE_PHYSICAL ||
        column == COL_ITEMREVIT_SIZE_LOGICAL ||
        column == COL_ITEMREVIT_LAST_CHANGE;
}

BEGIN_MESSAGE_MAP(CFileRevitControl, CTreeListControl)
    ON_WM_SETFOCUS()
    ON_WM_KEYDOWN()
    ON_NOTIFY_REFLECT_EX(LVN_DELETEALLITEMS, OnDeleteAllItems)
END_MESSAGE_MAP()

CFileRevitControl* CFileRevitControl::m_singleton = nullptr;

void CFileRevitControl::ProcessRevitFiles(CItem* item)
{
    // Do not process if we are not tracking large files
    // if (COptions::RevitFileCount == 0) return;

    m_queuedSet.push(item);
}

void CFileRevitControl::SortItems()
{
    ASSERT(AfxGetThread() != nullptr);

    // Verify at least root exists
    if (GetItemCount() == 0) return;

    // Record size and complete resort if top N changed
    const auto topN = static_cast<size_t>(COptions::RevitFileCount.Obj());
    if (topN != m_previousRevitN)
    {
        std::ranges::sort(m_sizeMap, CompareBySize);
        m_previousRevitN = topN;
        m_needsResort = true;
    }

    // Process queued items - only mark for resort if item could affect top N
    CItem* newItem = nullptr;
    while (m_queuedSet.pop(newItem))
    {
        // Check if this item could affect the top N
        if (m_sizeMap.size() < topN || newItem->GetSizeLogical() > m_revitNMinSize)
        {
            m_needsResort = true;
        }
        m_sizeMap.push_back(newItem);
    }

    // Only sort the vector if we need to update the top N
    if (!m_needsResort)
    {
        CTreeListControl::SortItems();
        return;
    }

    m_needsResort = false;
    const auto sortEnd = m_sizeMap.size() <= topN ? m_sizeMap.end()
        : m_sizeMap.begin() + topN;

    // Partial sort to get top N items at the front
    std::partial_sort(m_sizeMap.begin(), sortEnd, m_sizeMap.end(), CompareBySize);

    // Update minimum size in top N for future comparisons
    if (m_sizeMap.size() >= topN)
    {
        m_revitNMinSize = m_sizeMap[topN - 1]->GetSizeLogical();
    }
    else if (!m_sizeMap.empty())
    {
        m_revitNMinSize = m_sizeMap.back()->GetSizeLogical();
    }
    else
    {
        m_revitNMinSize = 0;
    }

    // Update visual item removals
    const auto root = reinterpret_cast<CItemRevit*>(GetItem(0));
    auto itemTrackerCopy = std::unordered_map(m_itemTracker);
    for (const auto& largeItem : m_sizeMap | std::views::take(topN))
    {
        if (m_itemTracker.contains(largeItem))
        {
            itemTrackerCopy.erase(largeItem);
            continue;
        }


        if (largeItem->IsTypeOrFlag(ITF_REVIT))
        {
            const auto itemRevit = new CItemRevit(largeItem);
            root->AddRevitItemChild(itemRevit);
            m_itemTracker[largeItem] = itemRevit;
        }
    }

    // Handle visual item additions
    for (const auto& itemRevit : itemTrackerCopy | std::views::values)
    {
        m_itemTracker.erase(itemRevit->GetLinkedItem());
        root->RemoveRevitItemChild(itemRevit);
    }

    CTreeListControl::SortItems();
}


void CFileRevitControl::RemoveItem(CItem* item)
{
    // Create list of all items to remove
    std::unordered_set<CItem*> toRemove;
    std::stack<CItem*> queue({ item });
    while (!queue.empty())
    {
        const auto qitem = queue.top();
        queue.pop();

        if (qitem->IsTypeOrFlag(IT_FILE))
        {
            toRemove.emplace(qitem);
        }
        else if (!qitem->IsLeaf()) for (const auto& child : qitem->GetChildren())
        {
            if (child->IsTypeOrFlag(IT_FILE)) toRemove.emplace(child);
            else queue.push(child);
        }
    }

    // Remove items in bulk
    m_needsResort = true;
    std::erase_if(m_sizeMap, [&](const auto& item)
    {
        return toRemove.contains(item);
    });

    // Use the sort function to remove visual items
    CMainFrame::Get()->InvokeInMessageThread([&]
    {
        SortItems();
    });
}

void CFileRevitControl::OnItemDoubleClick(const int i)
{
    if (const auto item = GetItem(i)->GetLinkedItem();
        item != nullptr && item->IsTypeOrFlag(IT_FILE))
    {
        CDirStatDoc::OpenItem(item);
    }
    else
    {
        CTreeListControl::OnItemDoubleClick(i);
    }
}

BOOL CFileRevitControl::OnDeleteAllItems(NMHDR*, LRESULT* pResult)
{
    // Reset trackers
    m_sizeMap.clear();
    m_itemTracker.clear();
    m_revitNMinSize = 0;
    m_needsResort = true;

    // Allow deletion to proceed
    *pResult = FALSE;
    return FALSE;
}

void CFileRevitControl::OnSetFocus(CWnd* pOldWnd)
{
    CTreeListControl::OnSetFocus(pOldWnd);
    CMainFrame::Get()->SetLogicalFocus(LF_REVITLIST);
}

void CFileRevitControl::OnKeyDown(const UINT nChar, const UINT nRepCnt, const UINT nFlags)
{
    if (nChar == VK_TAB)
    {
        CMainFrame::Get()->MoveFocus(LF_EXTLIST);
    }
    else if (nChar == VK_ESCAPE)
    {
        CMainFrame::Get()->MoveFocus(LF_NONE);
    }
    CTreeListControl::OnKeyDown(nChar, nRepCnt, nFlags);
}
