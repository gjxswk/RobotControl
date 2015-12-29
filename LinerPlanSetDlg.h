#if !defined(AFX_LINERPLANSETDLG_H__BFFE237D_78FE_4343_A051_AE0DF5ED2F75__INCLUDED_)
#define AFX_LINERPLANSETDLG_H__BFFE237D_78FE_4343_A051_AE0DF5ED2F75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinerPlanSetDlg.h : header file
//
#include "RobotControlView.h"
#include "RobotControlDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CLinerPlanSetDlg dialog

class CLinerPlanSetDlg : public CDialog
{
// Construction
public:
	CLinerPlanSetDlg(CWnd* pParent = NULL);   // standard constructor
	CRobotControlView* view;
	CRobotControlDoc* doc;
// Dialog Data
	//{{AFX_DATA(CLinerPlanSetDlg)
	enum { IDD = IDD_LINERPLANSETTING };
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
	//{{AFX_VIRTUAL(CLinerPlanSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinerPlanSetDlg)
	afx_msg void OnButtonImport();
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINERPLANSETDLG_H__BFFE237D_78FE_4343_A051_AE0DF5ED2F75__INCLUDED_)
