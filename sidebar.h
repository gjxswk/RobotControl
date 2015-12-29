#if !defined(AFX_SIDEBAR_H__44B7EEA3_4EE1_11D4_A0DC_00E09877609F__INCLUDED_)
#define AFX_SIDEBAR_H__44B7EEA3_4EE1_11D4_A0DC_00E09877609F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// sidebar.h : header file
//
#include "CtrlTab.h"
/////////////////////////////////////////////////////////////////////////////
// CSideBar window

class CSideBar : public CDialogBar
{
// Construction
public:
	CSideBar();

// Attributes
public:
	CCtrlTab *pCtrlTab;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSideBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSideBar();
	// Generated message map functions
protected:
	//{{AFX_MSG(CSideBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDEBAR_H__44B7EEA3_4EE1_11D4_A0DC_00E09877609F__INCLUDED_)
