#pragma once
#include "Layout.h"
#include "resource.h"


// CSearchOptionsDlg dialog

class CSearchOptionsDlg : public CLayoutDialogEx
{
    DECLARE_DYNAMIC(CSearchOptionsDlg)

public:
    CSearchOptionsDlg(CWnd* pParent = nullptr);   // standard constructor
    virtual ~CSearchOptionsDlg();

    enum : std::uint8_t { IDD = IDD_SEARCHRVT };

    BOOL m_RVTSearch = false;
    BOOL m_RFASearch = false;
    BOOL m_BACKUPSearch = false;

protected:
    BOOL OnInitDialog() override;
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    void OnOK();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


    CButton m_okButton;

    DECLARE_MESSAGE_MAP()
    // afx_msg void OnBnClickedUpdateButtons();
};
