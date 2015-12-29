// LinerPlanSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RobotControl.h"
#include "LinerPlanSetDlg.h"
#include "MainFrm.h"
#include "RobotControlView.h"
#include "RobotControlDoc.h"
#include "PathPlanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinerPlanSetDlg dialog


CLinerPlanSetDlg::CLinerPlanSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinerPlanSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinerPlanSetDlg)
	m_aifend = 0.0f;
	m_aifint = 0.0f;
	m_bitend = 0.0f;
	m_bitint = 0.0f;
	m_gamend = 0.0f;
	m_gamint = 0.0f;
	m_pxend = 0.0f;
	m_pxint = 0.0f;
	m_pyend = 0.0f;
	m_pyint = 0.0f;
	m_pzend = 0.0f;
	m_pzint = 0.0f;
	//}}AFX_DATA_INIT
}


void CLinerPlanSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinerPlanSetDlg)
	DDX_Text(pDX, IDC_EDIT_AIFEND, m_aifend);
	DDX_Text(pDX, IDC_EDIT_AIFINT, m_aifint);
	DDX_Text(pDX, IDC_EDIT_BITEND, m_bitend);
	DDX_Text(pDX, IDC_EDIT_BITINT, m_bitint);
	DDX_Text(pDX, IDC_EDIT_GAMEND, m_gamend);
	DDX_Text(pDX, IDC_EDIT_GAMINT, m_gamint);
	DDX_Text(pDX, IDC_EDIT_PXEND, m_pxend);
	DDX_Text(pDX, IDC_EDIT_PXINT, m_pxint);
	DDX_Text(pDX, IDC_EDIT_PYEND, m_pyend);
	DDX_Text(pDX, IDC_EDIT_PYINT, m_pyint);
	DDX_Text(pDX, IDC_EDIT_PZEND, m_pzend);
	DDX_Text(pDX, IDC_EDIT_PZINT, m_pzint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinerPlanSetDlg, CDialog)
	//{{AFX_MSG_MAP(CLinerPlanSetDlg)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinerPlanSetDlg message handlers

void CLinerPlanSetDlg::OnButtonImport() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	m_pxint=doc->ini_px;
	m_pyint=doc->ini_py;
	m_pzint=doc->ini_pz;
	m_aifint=doc->ini_aif;
	m_bitint=doc->ini_bit;
	m_gamint=doc->ini_gam;
    UpdateData(false);
	
}

void CLinerPlanSetDlg::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	UpdateData(true);
    doc->des_px=m_pxend;
    doc->des_py=m_pyend;
    doc->des_pz=m_pzend;
    doc->des_aif=m_aifend;
    doc->des_bit=m_bitend;
    doc->des_gam=m_gamend;
	((CButton*)GetDlgItem(IDOK))->EnableWindow(true);
	
}
