#if !defined(AFX_ROBOTCTRLPAN_H__F8F0CA61_D748_413A_9123_2BD3803FD987__INCLUDED_)
#define AFX_ROBOTCTRLPAN_H__F8F0CA61_D748_413A_9123_2BD3803FD987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotCtrlPan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobotCtrlPan window
#include "InitHardware.h"
#include "PathPlanDlg.h"

class CCtrlTab : public CTabCtrl
{
// Construction
public:
	CCtrlTab();

// Attributes
public:
	CInitHardware *pInitHardware;
	CPathPlanDlg *pPathPlanDlg;

// Operations
public:
   void HideAllDlg();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotCtrlPan)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCtrlTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRobotCtrlPan)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTCTRLPAN_H__F8F0CA61_D748_413A_9123_2BD3803FD987__INCLUDED_)
