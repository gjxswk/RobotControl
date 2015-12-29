#ifndef ROBOT_CONTROL_MULTI_DOC
#define ROBOT_CONTROL_MULTI_DOC

//#if !defined(AFX_ROBOTCONTROLDOCVIEW_H__AB27F144_DE28_46F1_94D6_05F53DAD6AFF__INCLUDED_)
//#define AFX_ROBOTCONTROLDOC_H__AB27F144_DE28_46F1_94D6_05F53DAD6AFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RobotControlDoc.h"


class CRobotControlMultiDoc :public CDocument
{
protected: // create from serialization only
	CRobotControlMultiDoc();
	DECLARE_DYNCREATE(CRobotControlMultiDoc)
public:
	CRobotControlDoc* doc_delay;	// doc of window showed for delay
	CRobotControlDoc* doc_real;		// doc of window showed for reality
public:
	  void RenderScene();
	  void PrintfBitmap( LPCTSTR lpszText, GLfloat x, GLfloat y, GLfloat z=0);
	  GLUquadricObj* obj;
	  HFONT m_hFont; //×ÖÌå¾ä±ú


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotControlDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRobotControlMultiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRobotControlDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif  // ROBOT_CONTROL_MULTI_DOC

