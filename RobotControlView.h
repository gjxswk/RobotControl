// RobotControlView.h : interface of the CRobotControlView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOTCONTROLVIEW_H__587FB4EA_7CED_4893_812D_74E0A92DF96E__INCLUDED_)
#define AFX_ROBOTCONTROLVIEW_H__587FB4EA_7CED_4893_812D_74E0A92DF96E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RobotControlMultiDoc.h"
//#include "PathPlanDlg.h"

class CPathPlanDlg;

class CRobotControlView : public CView
{
protected: // create from serialization only
	CRobotControlView();
	DECLARE_DYNCREATE(CRobotControlView)

// Attributes
public:
	HGLRC m_hGLContext; //着色描述表的句柄
	int m_GLPixelIndex;
	BOOL   m_LButtonDown,m_RButtonDown;
	CPoint m_LDownPos,m_RDownPos;
	bool display_revert;
public:
	CRobotControlMultiDoc* GetDocument();
	CPathPlanDlg* GetPathPlanDlg();

// Operations
public:
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotControlView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam); 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRobotControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRobotControlView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RobotControlView.cpp
inline CRobotControlMultiDoc* CRobotControlView::GetDocument()
   { return (CRobotControlMultiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTCONTROLVIEW_H__587FB4EA_7CED_4893_812D_74E0A92DF96E__INCLUDED_)
