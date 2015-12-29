// AutoPlanSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RobotControl.h"
#include "AutoPlanSetDlg.h"
#include "RobotControlView.h"
#include "RobotControlDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoPlanSetDlg dialog


CAutoPlanSetDlg::CAutoPlanSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoPlanSetDlg::IDD, pParent)               //---乐--自动规划设置函数
{
	//{{AFX_DATA_INIT(CAutoPlanSetDlg)
	m_aifend = 180.0f;
	m_aifint = 0.0f;
	m_bitend = 50.0f;
	m_bitint = 0.0f;
	m_gamend = -110.0f;
	m_gamint = 0.0f;
	m_pxend = 0.3f;
	m_pxint = 0.0f;
	m_pyend = 0.5f;
	m_pyint = 0.0f;
	m_pzend = 0.4f;
	m_pzint = 0.0f;
//	HandEyeFlag=false;
	CapOrientFlag=1;
	//}}AFX_DATA_INIT
}


void CAutoPlanSetDlg::DoDataExchange(CDataExchange* pDX)   //---乐--改变控制对话框数据函数
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoPlanSetDlg)
	DDX_Text(pDX, IDC_AIFEND, m_aifend);
	DDX_Text(pDX, IDC_AIFINT, m_aifint);
	DDX_Text(pDX, IDC_BITEND, m_bitend);
	DDX_Text(pDX, IDC_BITINT, m_bitint);
	DDX_Text(pDX, IDC_GAMEND, m_gamend);
	DDX_Text(pDX, IDC_GAMINT, m_gamint);
	DDX_Text(pDX, IDC_PXEND, m_pxend);
	DDX_Text(pDX, IDC_PXINT, m_pxint);
	DDX_Text(pDX, IDC_PYEND, m_pyend);
	DDX_Text(pDX, IDC_PYINT, m_pyint);
	DDX_Text(pDX, IDC_PZEND, m_pzend);
	DDX_Text(pDX, IDC_PZINT, m_pzint);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoPlanSetDlg, CDialog)  //---乐---宏定义的一种，位每个消息处理函数添加一个入口
	//{{AFX_MSG_MAP(CAutoPlanSetDlg)
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnBtnImport) //--乐--开始消息处理函数
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
//	ON_BN_CLICKED(IDC_CHECK_EYE, OnCheckEye)
	ON_BN_CLICKED(IDC_RAD_CAPORIENTX, OnRadCaporientx)
	ON_BN_CLICKED(IDC_RAD_CAPORIENTY, OnRadCaporienty)
	ON_BN_CLICKED(IDC_RAD_CAPORIENTZ, OnRadCaporientz)
	ON_BN_CLICKED(IDC_RAD_CAPORIENTXF, OnRadCaporientxf)
	ON_BN_CLICKED(IDC_RAD_CAPORIENTYF, OnRadCaporientyf)
	ON_BN_CLICKED(IDC_RAD_CAPORIENTZF, OnRadCaporientzf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()                            //---乐---宏定义结束

/////////////////////////////////////////////////////////////////////////////
// CAutoPlanSetDlg message handlers

void CAutoPlanSetDlg::OnBtnImport()     //--乐--添加按钮输入事件执行程序
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();  //--乐---获取view类指针
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

void CAutoPlanSetDlg::OnBtnSet()   //--乐--通知处理程序函数
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
    doc->des_px=m_pxend;
    doc->des_py=m_pyend;
    doc->des_pz=m_pzend;
    doc->des_aif=m_aifend;
    doc->des_bit=m_bitend;
    doc->des_gam=m_gamend;

//	doc->HandEyeFlag=HandEyeFlag;
	doc->CapOrientFlag=CapOrientFlag;
	((CButton*)GetDlgItem(IDOK))->EnableWindow(true);
	
}

BOOL CAutoPlanSetDlg::OnInitDialog()      //--乐--初始化函数
{
	CDialog::OnInitDialog();
	((CButton*)GetDlgItem(IDC_RAD_CAPORIENTZ))->SetCheck(BST_CHECKED);
	CapOrientFlag=3;
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
void CAutoPlanSetDlg::OnCheckEye() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument();
	
	if(((CButton*)GetDlgItem(IDC_CHECK_EYE))->GetCheck()==0)
	{	
		HandEyeFlag=false;  
//		doc->HandEyeFlag=HandEyeFlag;
		
		((CEdit*)GetDlgItem(IDC_PXEND))->EnableWindow(true);
		((CEdit*)GetDlgItem(IDC_PYEND))->EnableWindow(true);
		((CEdit*)GetDlgItem(IDC_PZEND))->EnableWindow(true);
		((CEdit*)GetDlgItem(IDC_AIFEND))->EnableWindow(true);
		((CEdit*)GetDlgItem(IDC_BITEND))->EnableWindow(true);
		((CEdit*)GetDlgItem(IDC_GAMEND))->EnableWindow(true);
		
	}
	else if(((CButton*)GetDlgItem(IDC_CHECK_EYE))->GetCheck()==1)
	{   
		HandEyeFlag=true; 
//		doc->HandEyeFlag=HandEyeFlag;
		
		((CEdit*)GetDlgItem(IDC_PXEND))->EnableWindow(false);
		((CEdit*)GetDlgItem(IDC_PYEND))->EnableWindow(false);
		((CEdit*)GetDlgItem(IDC_PZEND))->EnableWindow(false);
		((CEdit*)GetDlgItem(IDC_AIFEND))->EnableWindow(false);
		((CEdit*)GetDlgItem(IDC_BITEND))->EnableWindow(false);
		((CEdit*)GetDlgItem(IDC_GAMEND))->EnableWindow(false);
	} 


	
}
*/

void CAutoPlanSetDlg::OnRadCaporientx() //--乐--X旋转角度
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	CapOrientFlag=1;
	doc->CapOrientFlag=CapOrientFlag;
	
}

void CAutoPlanSetDlg::OnRadCaporienty()  //--乐--y旋转角度
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	CapOrientFlag=2;
	doc->CapOrientFlag=CapOrientFlag;
	
}

void CAutoPlanSetDlg::OnRadCaporientz()  //--乐--z旋转角度
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	CapOrientFlag=3;	
	doc->CapOrientFlag=CapOrientFlag;
	
}

void CAutoPlanSetDlg::OnRadCaporientxf() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	CapOrientFlag=4;	
	doc->CapOrientFlag=CapOrientFlag;
	
}

void CAutoPlanSetDlg::OnRadCaporientyf() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	CapOrientFlag=5;	
	doc->CapOrientFlag=CapOrientFlag;

	
}

void CAutoPlanSetDlg::OnRadCaporientzf() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	CapOrientFlag=6;	
	doc->CapOrientFlag=CapOrientFlag;

	
}
