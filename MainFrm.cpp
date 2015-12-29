// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RobotControl.h"

#include "MainFrm.h"
#include <afxsock.h>
#include "m5apiw32.h"

// added by gjx
#include "conio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DWORD WINAPI recv_cam_data1(LPVOID dlg)
{
	AfxSocketInit();   //---��--�����׽��֣��д�ȷ�ϣ�
	CString IP;
	IP="59.64.138.61";

	CRobotControlView* view;
	CRobotControlDoc* doc;

	CMainFrame* pframe=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)pframe->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	UINT port;
	port=9300;
	float data_str[255];//��Ž��յ��Ľ�Ҫ���������ݵĴ�С
	CSocket m_socket,cliSock;
	m_socket.Create(port);
	//���ӵ����Ͷ�
	if(!m_socket.Connect(IP, port))
	{
		AfxMessageBox(_T("�޷����ӵ�������!"));
//		//�ر��׽���
		m_socket.Close();
		return -1;
	}
	char data[255];
	m_socket.Listen();   //---��---�����׽���
	m_socket.Accept(cliSock);
	while(!pframe->m_exit)
	{
		AllocConsole();
		_cprintf("Here for waiting..\n");
	cliSock.Receive(data,255,0);
	_cprintf("get thd data: %s\n", data);
	FreeConsole();
	

	// added by gjx
	FILE* port_file = fopen("mes_port.txt", "w");
	fprintf(port_file, "%X\n", data);
	// ended

	pframe->pSiderBar->pCtrlTab->pPathPlanDlg->ControlMode=1;
	unsigned char ColStatusWord=data[0];
	unsigned char KeyWord=data[1];
	int device=pframe->pSiderBar->pCtrlTab->pInitHardware->m_device;
	char SendData[255];
	float ceta[8];
	int i,j;

	switch (ColStatusWord)
	{
	case 128://��ʼ��
		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_PMACOPEN)->SendMessage(BM_CLICK,1,1);
		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_SCAN)->SendMessage(BM_CLICK,1,1);
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_RADIO_JXB)->SendMessage(BM_CLICK,1,1);
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_SET)->SendMessage(BM_CLICK,1,1);//��������ť
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_INIT)->SendMessage(BM_CLICK,1,1);
		//pframe->pSiderBar->pCtrlTab->pPathPlanDlg->OnButtonSet();
		break;
	case 64://��λ
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_RESET)->SendMessage(BM_CLICK,1,1);
		break;
	case 32://ֹͣ
		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_PMACCLOSE)->SendMessage(BM_CLICK,1,1);
//		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_EXIT)->SendMessage(BM_CLICK,1,1);
		pframe->OnStopConnect();
//		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_STOP)->SendMessage(BM_CLICK,1,1);
		break;
	case 40://�ֶ�������ͣ
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_STOPH)->SendMessage(BM_CLICK,1,1);
		break;
	case 36://�����滮��ͣ
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_STOP)->SendMessage(BM_CLICK,1,1);
		break;
	case 16://��������
		float actpos[8];
		float actvel[8];
		
		while(1)
		{
			for (i=1;i<8;i++)
			{
				PCube_getPos(device,i,&actpos[i]);  //---��--��ȡλ����Ϣ��д������
				PCube_getVel(device,i,&actvel[i]);   //---��--��ȡ�ٶ���Ϣ��д������
			}
			memcpy(SendData,actpos,32);             //---��---����λ��32λ��Ϣ���������ݻ�������
			memcpy(SendData+32,actvel,32);          //---��---�����ٶ�32λ��Ϣ���������ݻ�������
			m_socket.Send(SendData,255,0);          //---��---���ͻ��������п���ָ��
			Sleep(100);                             //---��---����100����
		}
/*		for (i=0;i<pframe->pSiderBar->pCtrlTab->pPathPlanDlg->kn;i++)
		{
			for(j=0;j<8;j++)
			{
				ceta[j]=pframe->pSiderBar->pCtrlTab->pPathPlanDlg->ceta[i][j];
			}
			memcpy(SendData,ceta,32);
			m_socket.Send(SendData,255,0);
			Sleep(100);
		}
*/				
		break;
	case 8://�ֶ�����
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_SETH)->SendMessage(BM_CLICK,1,1);
		float ScrValue;
		memcpy(&ScrValue,data+2,4);
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->ScrValue=ScrValue;
		switch(KeyWord)
		{
			case 128://X����ƽ��
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_TRANSLATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_XTRANSP)->SendMessage(BM_CLICK,1,1);		
				break;
			case 130://x������ת
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_ROTATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_XROTP)->SendMessage(BM_CLICK,1,1);
				break;
			case 64://x����ƽ��
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_TRANSLATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_XTRANSI)->SendMessage(BM_CLICK,1,1);
				break;
			case 66://x������ת
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_ROTATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_XROTI)->SendMessage(BM_CLICK,1,1);
				break;
			case 32://Y����ƽ��
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_TRANSLATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_YTRANSP)->SendMessage(BM_CLICK,1,1);
				break;
			case 34://Y������ת
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_ROTATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_YROTP)->SendMessage(BM_CLICK,1,1);
				break;
			case 16://Y����ƽ��
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_TRANSLATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_YTRANSI)->SendMessage(BM_CLICK,1,1);
				break;
			case 18://Y������ת
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_ROTATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_YROTI)->SendMessage(BM_CLICK,1,1);
				break;
			case 8://z����ƽ��
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_TRANSLATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_ZTRANSP)->SendMessage(BM_CLICK,1,1);
				break;
			case 10://z������ת
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_ROTATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_ZROTP)->SendMessage(BM_CLICK,1,1);
				break;
			case 4://z����ƽ��
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_TRANSLATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_ZTRANSI)->SendMessage(BM_CLICK,1,1);
				break;
			case 6://z������ת
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_RADIO_ROTATE)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pHandControlDlg->GetDlgItem(IDC_BUTTON_ZROTI)->SendMessage(BM_CLICK,1,1);
				break;
			default:break;
		}

		break;
	case 4://�����滮
		float posture[6];
		for (i=0;i<6;i++)
		{
			memcpy(&posture[i],data+2+4*i,4);
		}
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->m_pxend=posture[0];
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->m_pyend=posture[1];
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->m_pzend=posture[2];
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->m_aifend=posture[3];
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->m_bitend=posture[4];
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->m_gamend=posture[5];

		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_RADIO_AUTOPLAN)->SendMessage(BM_CLICK,1,1);
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDC_BTN_IMPORT)->SendMessage(BM_CLICK,1,1);
		pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDC_BTN_SET)->SendMessage(BM_CLICK,1,1);
		switch(KeyWord)
		{
			case 128://X������������
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDC_RAD_CAPORIENTX)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_INIOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_RUN)->SendMessage(BM_CLICK,1,1);
				break;

			case 64://x������������
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDC_RAD_CAPORIENTXF)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_INIOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_RUN)->SendMessage(BM_CLICK,1,1);
				break;

			case 32://Y������������
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDC_RAD_CAPORIENTY)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_INIOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_RUN)->SendMessage(BM_CLICK,1,1);
				break;

			case 16://Y������������
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDC_RAD_CAPORIENTYF)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_INIOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_RUN)->SendMessage(BM_CLICK,1,1);
				break;

			case 8://z������������
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDC_RAD_CAPORIENTZ)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_INIOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_RUN)->SendMessage(BM_CLICK,1,1);
				break;

			case 4://z������������
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDC_RAD_CAPORIENTZF)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->pAutoPlanSetDlg->GetDlgItem(IDOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_INIOK)->SendMessage(BM_CLICK,1,1);
				pframe->pSiderBar->pCtrlTab->pPathPlanDlg->GetDlgItem(IDC_BUTTON_RUN)->SendMessage(BM_CLICK,1,1);
				break;

			default:break;
		}

		break;
	default:break;
	}
	}
	m_socket.Close();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_CONNECT_COMPUTER, OnConnectComputer)
	ON_COMMAND(ID_STOP_CONNECT, OnStopConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_exit=false;
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	pSiderBar=new CSideBar;
	pSiderBar->Create(this, IDD_SIDEBAR, CBRS_RIGHT, IDD_SIDEBAR);
	pSiderBar->SetBarStyle(pSiderBar->GetBarStyle() | CBRS_BORDER_RIGHT);
	pSiderBar->EnableDocking(CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_RIGHT);
	DockControlBar(pSiderBar);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	int cxScreen=::GetSystemMetrics(SM_CXSCREEN);   
	int cyScreen=::GetSystemMetrics(SM_CYSCREEN);
	cs.x=0;
	cs.y=0;
	cs.cx=cxScreen;
	cs.cy=cyScreen;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnConnectComputer() 
{
	// TODO: Add your command handler code here
	m_exit=false;
	HANDLE pThread1;
	pThread1=CreateThread(NULL,0,recv_cam_data1,(LPVOID)this,0,NULL);//ȫ�����
	CloseHandle(pThread1);
	
}

void CMainFrame::OnStopConnect() 
{
	// TODO: Add your command handler code here
	CMainFrame* pframe=(CMainFrame*)AfxGetApp()->GetMainWnd();
	pframe->m_exit=true;
	
}

void CMainFrame::OnAutoplan() 
{
	// TODO: Add your command handler code here
	
}
