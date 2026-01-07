// CSearchOptionsDlg.cpp : implementation file
//

#include "pch.h"
#include "resource.h"
#include "SearchOptionsDlg.h"
#include "version.h"
#include "RvtDirStat.h"
#include "Constants.h"
#include "Localization.h"
#include "Options.h"
#include "GlobalHelpers.h"

// CSearchOptionsDlg dialog

IMPLEMENT_DYNAMIC(CSearchOptionsDlg, CLayoutDialogEx)

CSearchOptionsDlg::CSearchOptionsDlg(CWnd* pParent /*=nullptr*/)
    // : CLayoutDialogEx(IDD, COptions::DriveSelectWindowRect.Ptr(), pParent)
    : CLayoutDialogEx(IDD, COptions::SearchRvtWindowRect.Ptr(), pParent)
{

}

// Implementation of destructor
CSearchOptionsDlg::~CSearchOptionsDlg()
{
}

void CSearchOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
    CLayoutDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_RVT_SEARCH, m_RVTSearch);
    DDX_Check(pDX, IDC_RFA_SEARCH, m_RFASearch);
    DDX_Check(pDX, IDC_BACKUP_SEARCH, m_BACKUPSearch);
    DDX_Control(pDX, IDOK, m_okButton);

}

BEGIN_MESSAGE_MAP(CSearchOptionsDlg, CLayoutDialogEx)
    // ON_BN_CLICKED(IDC_RVT_SEARCH, OnBnClickedUpdateButtons)
    // ON_BN_CLICKED(IDC_RFA_SEARCH, OnBnClickedUpdateButtons)
    // ON_BN_CLICKED(IDC_BACKUP_SEARCH, OnBnClickedUpdateButtons)
    ON_WM_DESTROY()
    ON_WM_GETMINMAXINFO()
    ON_WM_MEASUREITEM()
    ON_WM_SIZE()
    ON_WM_SYSCOLORCHANGE()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL CSearchOptionsDlg::OnInitDialog()
{
    COptions::SearchRvtWindowRect.Obj() = { 548, 278, 1052, 444 };

    CLayoutDialogEx::OnInitDialog();

    Localization::UpdateDialogs(*this);
    DarkMode::AdjustControls(GetSafeHwnd());

    ModifyStyle(0, WS_CLIPCHILDREN);

    m_layout.AddControl(IDOK, 1, 0, 0, 0);
    m_layout.AddControl(IDCANCEL, 1, 0, 0, 0);
    m_layout.AddControl(IDC_RVT_SEARCH, 0, 0, 1, 0);
    m_layout.AddControl(IDC_RFA_SEARCH, 0, 0, 1, 0);
    m_layout.AddControl(IDC_BACKUP_SEARCH, 0, 0, 1, 0);

    m_layout.OnInitDialog(true);

    return TRUE;
}

HBRUSH CSearchOptionsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, const UINT nCtlColor)
{
    const HBRUSH brush = DarkMode::OnCtlColor(pDC, nCtlColor);
    return brush ? brush : CLayoutDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

// CSearchOptionsDlg message handlers

void CSearchOptionsDlg::OnOK()
{
    UpdateData();

    // Clear existing filters if you want this dialog to be an "exclusive" allow list
    COptions::FilteringExcludeFilesRegex.clear();
    COptions::FilteringAllowFilesRegex.clear();

    if (m_RVTSearch)
    {
        // Simple match for .rvt files
        COptions::FilteringAllowFilesRegex.emplace_back(L".*\\.rvt$",
        std::regex_constants::icase | std::regex_constants::optimize);
        // COptions::FilteringAllowFilesRegex.emplace_back(L".*\\.[0-9]{4}\\.rvt$",
        //     std::regex_constants::icase | std::regex_constants::optimize);
    }

    if (m_RFASearch)
    {
        // Simple match for .rfa files
        COptions::FilteringAllowFilesRegex.emplace_back(L".*\\.rfa$",
        std::regex_constants::icase | std::regex_constants::optimize);
        // COptions::FilteringAllowFilesRegex.emplace_back(L".*\\.[0-9]{4}\\.rfa$",
        //     std::regex_constants::icase | std::regex_constants::optimize);
    }

    if (m_BACKUPSearch)
    {
        // The $ ensures the entire path (including the extension) is accounted for
        COptions::FilteringAllowFilesRegex.emplace_back(L".*_backup[\\\\/][^\\\\/]+$",
            std::regex_constants::icase | std::regex_constants::optimize);
    }

    // COptions::CompileFilters();

    CLayoutDialogEx::OnOK();
}
