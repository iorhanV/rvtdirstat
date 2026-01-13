
#include "pch.h"
#include "ItemRevit.h"
#include "FileRevitControl.h"

CItemRevit::CItemRevit(CItem* item) : m_item(item) {}

CItemRevit::~CItemRevit()
{
    for (const auto& m_child : m_children)
    {
        delete m_child;
    }
}

const std::unordered_map<uint8_t, uint8_t> CItemRevit::s_columnMap =
{
    { COL_ITEMREVIT_NAME, COL_NAME },
    { COL_ITEMREVIT_SIZE_LOGICAL, COL_SIZE_LOGICAL },
    { COL_ITEMREVIT_SIZE_PHYSICAL, COL_SIZE_PHYSICAL },
    { COL_ITEMREVIT_LAST_CHANGE, COL_LAST_CHANGE }
};

bool CItemRevit::DrawSubItem(const int subitem, CDC* pdc, const CRect rc, const UINT state, int* width, int* focusLeft)
{
    // Handle individual file items
    if (subitem != COL_ITEMREVIT_NAME) return false;
    return CTreeListItem::DrawSubItem(s_columnMap.at(static_cast<uint8_t>(subitem)), pdc, rc, state, width, focusLeft);
}

std::wstring CItemRevit::GetText(const int subitem) const
{
    // Root node
    static std::wstring tops = Localization::Lookup(IDS_REVIT_FILES);
    if (GetParent() == nullptr) return subitem == COL_ITEMREVIT_NAME ?
        std::format(L"{} ({})", tops, m_children.size()) : std::wstring{};

    // Parent hash nodes
    if (m_item == nullptr) return {};

    // Individual file names
    if (subitem == COL_ITEMREVIT_NAME) return m_item->GetPath();
    return m_item->GetText(s_columnMap.at(static_cast<uint8_t>(subitem)));
}

int CItemRevit::CompareSibling(const CTreeListItem* tlib, const int subitem) const
{
    // Root node
    if (GetParent() == nullptr) return 0;

    // Parent hash nodes
    if (m_item == nullptr) return 0;

    // Individual file names
    const auto* other = reinterpret_cast<const CItemRevit*>(tlib);
    return m_item->CompareSibling(other->m_item, s_columnMap.at(static_cast<uint8_t>(subitem)));
}

int CItemRevit::GetTreeListChildCount()const
{
    return static_cast<int>(m_children.size());
}

CTreeListItem* CItemRevit::GetTreeListChild(const int i) const
{
    return m_children[i];
}

COLORREF CItemRevit::GetItemTextColor() const
{
    if (m_item != nullptr)
    {
        // Logic to grey out main Revit files that aren't backups in the Top Files list
        if (m_item->IsTypeOrFlag(ITF_REVIT))
        {
            if (!m_item->IsTypeOrFlag(ITF_BACKUP))
            {
                return RGB(128, 128, 128);
            }
        }

        return m_item->GetItemTextColor();
    }
    return CTreeListItem::GetItemTextColor();
}

HICON CItemRevit::GetIcon()
{
    // No icon to return if not visible yet
    if (m_visualInfo == nullptr)
    {
        return nullptr;
    }

    // Return previously cached value
    if (m_visualInfo->icon != nullptr)
    {
        return m_visualInfo->icon;
    }

    // Cache icon for parent nodes
    if (m_item == nullptr)
    {
        m_visualInfo->icon = GetIconHandler()->GetRevitImage();
        return m_visualInfo->icon;
    }

    // Fetch all other icons
    CDirStatApp::Get()->GetIconHandler()->DoAsyncShellInfoLookup(std::make_tuple(this,
        m_visualInfo->control, m_item->GetPath(), m_item->GetAttributes(), &m_visualInfo->icon, nullptr));
    return nullptr;
}

void CItemRevit::AddRevitItemChild(CItemRevit* child)
{
    child->SetParent(this);

    std::scoped_lock guard(m_protect);
    m_children.push_back(child);

    if (IsVisible() && IsExpanded())
    {
        CFileRevitControl::Get()->OnChildAdded(this, child);
    }
}

void CItemRevit::RemoveRevitItemChild(CItemRevit* child)
{
    if (IsVisible())
    {
        CFileRevitControl::Get()->OnChildRemoved(this, child);
    }

    std::scoped_lock guard(m_protect);
    auto& children = m_children;
    if (auto it = std::ranges::find(children, child); it != children.end())
    {
        children.erase(it);
    }

    delete child;
}

void CItemRevit::RemoveRevitItemResults()
{
    for (const auto& m_child : std::vector(m_children))
    {
        RemoveRevitItemChild(m_child);
    }
}
