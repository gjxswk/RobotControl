#if !defined(AFX_AUTOPLANSETDLG_H__EAD17F0F_57B4_4071_A4E0_3D43B2CA76CD__INCLUDED_)
#define AFX_AUTOPLANSETDLG_H__EAD17F0F_57B4_4071_A4E0_3D43B2CA76CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoPlanSetDlg.h : header file
//
#include "RobotControlDoc.h"
#include "RobotControlView.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoPlanSetDlg dialog

class CAutoPlanSetDlg : public CDialog
{
// Construction
public:
	CAutoPlanSetDlg(CWnd* pParent = NULL);   // standard constructor
	CRobotControlView* view;
	CRobotControlDoc* doc;
public:
	bool HandEyeFlag;
	int CapOrientFlag;

// Dialog Data
	//{{AFX_DATA(CAutoPlanSetDlg)
	enum { IDD = IDD_AUTOPLANSETTING };
	float	m_aifend;
	float	m_aifint;
	float	m_bitend;
	float	m_bitint;
	float	m_gamend;
	float	m_gamint;
	float	m_pxend;
	float	m_pxint;
	float	m_pyend;
	float	m_pyint;
	float	m_pzend;
	float	m_pzint;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoPlanSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoPlanSetDlg)
	afx_msg void OnBtnImport();
	afx_msg void OnBtnSet();
	virtual BOOL OnInitDialog();
//	afx_msg void OnCheckEye();
	afx_msg void OnRadCaporientx();
	afx_msg void OnRadCaporienty();
	afx_msg void OnRadCaporientz();
	afx_msg void OnRadCaporientxf();
	afx_msg void OnRadCaporientyf();
	afx_msg void OnRadCaporientzf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOPLANSETDLG_H__EAD17F0F_57B4_4071_A4E0_3D43B2CA76CD__INCLUDED_)
