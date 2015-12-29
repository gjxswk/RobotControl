// RobotCtrlPan.cpp : implementation file
//

#include "stdafx.h"
#include "CtrlTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRobotCtrlPan

CCtrlTab::CCtrlTab()      //--乐--控制标签
{
	pInitHardware=NULL;  //--乐--CInitHardware 指针成员
	pPathPlanDlg=NULL;
}

CCtrlTab::~CCtrlTab()   //--乐--析构函数，释放相应资源
{
	if(pInitHardware != NULL)
		delete pInitHardware;
	pInitHardware = NULL; 
	if(pPathPlanDlg != NULL)
		delete pPathPlanDlg;
	pPathPlanDlg = NULL; 
}


BEGIN_MESSAGE_MAP(CCtrlTab, CTabCtrl)   //--乐--宏定义添加消息处理函数的入口
	//{{AFX_MSG_MAP(CRobotCtrlPan)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()       //--乐--宏定义结束

/////////////////////////////////////////////////////////////////////////////
// CRobotCtrlPan message handlers

int CCtrlTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	pInitHardware = new CInitHardware;  //--乐--有错误，原因？？？
	pInitHardware->Create(IDD_INITHARDWARE,this);//--乐--有错误，原因？？？

	pPathPlanDlg=new CPathPlanDlg;
	pPathPlanDlg->Create(IDD_PATHPLAN,this);


	TC_ITEM t_item;

	t_item.mask=TCIF_TEXT;
	t_item.pszText="InitHardware";  //--乐--标签上文本信息  
	InsertItem(0,&t_item);

	t_item.mask=TCIF_TEXT;
	t_item.pszText="PathPlan";  //--乐--标签上文本信息
	InsertItem(1,&t_item);

	RECT rect;
	this->GetClientRect(&rect);

	pInitHardware->MoveWindow(3, 24, rect.right-6, rect.bottom-20);//--乐--硬件初始化容器相对位置的变化
	pInitHardware->ShowWindow(true); 
	
	pPathPlanDlg->MoveWindow(3, 24, rect.right-6, rect.bottom-20);//--乐--路径规划容器相对位置的变化
	pPathPlanDlg->ShowWindow(false); 

	// TODO: Add your specialized creation code here
	
	
	return 0;
}

void CCtrlTab::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int tab_index;           //--乐---索引变量
	tab_index=GetCurSel();  //--乐---获取鼠标当前位置的索引值
	HideAllDlg();
	switch(tab_index)
	{
	case 0:	//display Inverse kinematics
	pInitHardware->ShowWindow(TRUE);       //--乐--显示硬件初始化对话框
		break; 
	case 1:
		pPathPlanDlg->ShowWindow(TRUE);   //--乐--显示路径规划对话框
		break;
	default:
		break;  
	}
	*pResult = 0;
}

void CCtrlTab::HideAllDlg()
{
//	pForKinDlg->ShowWindow(FALSE);
//	pPathPlanDlg->ShowWindow(FALSE);
	pInitHardware->ShowWindow(FALSE);
	pPathPlanDlg->ShowWindow(FALSE);
	//MessageBox("函数执行完成");//--乐--验证函数是否执行实验:切换标签将对应的窗口显示另一个窗口隐藏
}
