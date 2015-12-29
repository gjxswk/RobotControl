// sidebar.cpp : implementation file
//
#include "stdafx.h"
#include "sidebar.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSideBar

CSideBar::CSideBar()
{
}

CSideBar::~CSideBar()
{
}


BEGIN_MESSAGE_MAP(CSideBar, CDialogBar)
	//{{AFX_MSG_MAP(CSideBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CSideBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;	
	pCtrlTab=new CCtrlTab;
	RECT rect;
	GetClientRect(&rect);
	rect.top+=10; rect.left+=5; rect.bottom-=5; rect.right-=3;
	pCtrlTab->Create(TCS_TABS, rect, this, IDC_PARAMETER_TAB);
	pCtrlTab->ShowWindow(TRUE); 
	return 0;
}

