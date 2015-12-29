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

CCtrlTab::CCtrlTab()      //--��--���Ʊ�ǩ
{
	pInitHardware=NULL;  //--��--CInitHardware ָ���Ա
	pPathPlanDlg=NULL;
}

CCtrlTab::~CCtrlTab()   //--��--�����������ͷ���Ӧ��Դ
{
	if(pInitHardware != NULL)
		delete pInitHardware;
	pInitHardware = NULL; 
	if(pPathPlanDlg != NULL)
		delete pPathPlanDlg;
	pPathPlanDlg = NULL; 
}


BEGIN_MESSAGE_MAP(CCtrlTab, CTabCtrl)   //--��--�궨�������Ϣ�����������
	//{{AFX_MSG_MAP(CRobotCtrlPan)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()       //--��--�궨�����

/////////////////////////////////////////////////////////////////////////////
// CRobotCtrlPan message handlers

int CCtrlTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	pInitHardware = new CInitHardware;  //--��--�д���ԭ�򣿣���
	pInitHardware->Create(IDD_INITHARDWARE,this);//--��--�д���ԭ�򣿣���

	pPathPlanDlg=new CPathPlanDlg;
	pPathPlanDlg->Create(IDD_PATHPLAN,this);


	TC_ITEM t_item;

	t_item.mask=TCIF_TEXT;
	t_item.pszText="InitHardware";  //--��--��ǩ���ı���Ϣ  
	InsertItem(0,&t_item);

	t_item.mask=TCIF_TEXT;
	t_item.pszText="PathPlan";  //--��--��ǩ���ı���Ϣ
	InsertItem(1,&t_item);

	RECT rect;
	this->GetClientRect(&rect);

	pInitHardware->MoveWindow(3, 24, rect.right-6, rect.bottom-20);//--��--Ӳ����ʼ���������λ�õı仯
	pInitHardware->ShowWindow(true); 
	
	pPathPlanDlg->MoveWindow(3, 24, rect.right-6, rect.bottom-20);//--��--·���滮�������λ�õı仯
	pPathPlanDlg->ShowWindow(false); 

	// TODO: Add your specialized creation code here
	
	
	return 0;
}

void CCtrlTab::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int tab_index;           //--��---��������
	tab_index=GetCurSel();  //--��---��ȡ��굱ǰλ�õ�����ֵ
	HideAllDlg();
	switch(tab_index)
	{
	case 0:	//display Inverse kinematics
	pInitHardware->ShowWindow(TRUE);       //--��--��ʾӲ����ʼ���Ի���
		break; 
	case 1:
		pPathPlanDlg->ShowWindow(TRUE);   //--��--��ʾ·���滮�Ի���
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
	//MessageBox("����ִ�����");//--��--��֤�����Ƿ�ִ��ʵ��:�л���ǩ����Ӧ�Ĵ�����ʾ��һ����������
}
