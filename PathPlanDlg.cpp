// PathPlanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RobotControl.h"
#include "PathPlanDlg.h"
#include "MainFrm.h"
#include "Dynamic_spacerobot.h"
#include "math.h"
#include "m5apiw32.h"
#include "InitHardware.h"
#include "conio.h"
#include "mmsystem.h"
#include "XnCppWrapper.h"
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


double ini_ang[8], basiniPE[6], PEint[6], PEmid[6], PEend[6], now_ang[8],now_PEca[6], qdes[8];
double next_ang[8], next_angvel[8], next_baspe[6], next_basvel[6];
double EndPE[Ni][6];
double VelocityLimit[6]={0,0,0,0,0,0};
#define CODEANGTRANS 22.7555555555555556
#define PI 3.14159265359
#define DIAMETER 0.058
#define COMPILE false

// double size[6]={0,0.3,0.328,0.277,0.206,0.1};
// double dh[8][4]={{0,-PI/2,d1,0},{0,PI/2,d2,0},{0,-PI/2,0,0},{0,PI/2,d3,0},{0,-PI/2,0,0},{0,PI/2,d4,0},{0,-PI/2,0,0},{0,PI/2,d5,0}};

/////////////////////////////////////////////////////////////////////////////
// CPathPlanDlg dialog

using namespace xn;

CPathPlanDlg::CPathPlanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPathPlanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPathPlanDlg)
	JOINT_NUM = 8;
	m_JNT = new float[JOINT_NUM];
	m_JNT[0] = 0.0f;
	m_JNT[1] = 0.0f;
	m_JNT[2] = 0.0f;
	m_JNT[3] = 0.0f;
	m_JNT[4] = 0.0f;
	m_JNT[5] = 0.0f;
	m_JNT[6] = 0.0f;
	m_JNT[7] = 0.0f;
	for (int i = 0; i < 8; i++) {
		ini_ang[i] = m_JNT[i];
	}
	m_ts = 10;
	m_time = 80;
	m_ini_j0 = 0.0f;
	m_ini_j1 = 0.0f;
	m_ini_j2 = 0.0f;
	m_ini_j3 = 0.0f;
	m_ini_j4 = 0.0f;
	m_ini_j5 = 0.0f;
	m_ini_j6 = 0.0f;
	m_ini_j7 = 0.0f;
	m_baspx = 0.0f;
	m_baspy = 0.0f;
	m_baspz = 0.0f;
	m_basaif = 0.0f;
	m_basbit = 0.0f;
	m_basgam = 0.0f;
	kn=0;
	No=0;
	numtime=0;
	PreProgrammFlag=0;
	CtsVel=1;
	PMacdevice=0;
	
	predict_time = 0.00;
	predict_time_error = 0.00;

	int POINT_NUM = 8;
	des_angle = new double[POINT_NUM];
	des_vel = new double[POINT_NUM];
	des_acc = new double[POINT_NUM];
	former_angle = new double[POINT_NUM];

	// added by gjx
	ctrl_mode = 0;
	btn_release = false;

	if (COMPILE) {
		AllocConsole();
	}
	//AllocConsole();
	//}}AFX_DATA_INIT
}

CPathPlanDlg::~CPathPlanDlg() {
	if (COMPILE) {
		FreeConsole();
	}
}

void CPathPlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathPlanDlg)
	DDX_Text(pDX, IDC_EDIT_JNT1, m_JNT[1]);
	DDX_Text(pDX, IDC_EDIT_JNT2, m_JNT[2]);
	DDX_Text(pDX, IDC_EDIT_JNT3, m_JNT[3]);
	DDX_Text(pDX, IDC_EDIT_JNT4, m_JNT[4]);
	DDX_Text(pDX, IDC_EDIT_JNT5, m_JNT[5]);
	DDX_Text(pDX, IDC_EDIT_JNT6, m_JNT[6]);
	DDX_Text(pDX, IDC_EDIT_JNT7, m_JNT[7]);
	DDX_Text(pDX, IDC_EDIT_ACCTIME, m_ts);
	DDX_Text(pDX, IDC_EDIT_PLANTIME, m_time);
	DDX_Text(pDX, IDC_EDIT_JNT0, m_JNT[0]);
	//}}AFX_DATA_MAP
}

	

BEGIN_MESSAGE_MAP(CPathPlanDlg, CDialog)
	//{{AFX_MSG_MAP(CPathPlanDlg)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_INIT, OnButtonInit)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RADIO_LINEARPLAN, OnRadioLinearplan)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_INIOK, OnButtonIniok)
	ON_BN_CLICKED(IDC_BUTTON_RUN, OnButtonRun)
	ON_BN_CLICKED(IDC_RADIO_AUTOPLAN, OnRadioAutoplan)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_RADIO_HANDCONTROL, OnRadioHandcontrol)
	ON_BN_CLICKED(IDC_RADIO_SUMU, OnRadioSumu)
	ON_BN_CLICKED(IDC_RADIO_JXB, OnRadioJxb)
	ON_BN_CLICKED(IDC_BUTTON_SAVEDATA, OnButtonSavedata)
	ON_BN_CLICKED(IDC_RADIO_PROGRAMM, OnRadioProgramm)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_KEY_CONTROL, &CPathPlanDlg::OnButtonKeyControl)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_BN_CLICKED(IDC_CAMERA, &CPathPlanDlg::OnButtonCamera)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathPlanDlg message handlers

BOOL CPathPlanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT0))->SetRange(-1000,1000,true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT0))->SetTicFreq(200);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT0))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT1))->SetRange(-180,180,true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT1))->SetTicFreq(36);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT1))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT2))->SetRange(-90,90,true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT2))->SetTicFreq(18);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT2))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT3))->SetRange(-180,180,true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT3))->SetTicFreq(36);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT3))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT4))->SetRange(-120,120,true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT4))->SetTicFreq(24);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT4))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT5))->SetRange(-180,180,true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT5))->SetTicFreq(36);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT5))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT6))->SetRange(-150,150,true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT6))->SetTicFreq(30);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT6))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT7))->SetRange(-180,180,true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT7))->SetTicFreq(36);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT7))->SetPos(0);
	
	ControlMode=0;
	((CButton*)GetDlgItem(IDC_RADIO_SUMU))->SetCheck(true);
	//初始化参数
 //	init_robot_size(size);
 //	init_dh(dh);
// 	R0c[0][0]=T0C[0][0]=1;/*变换矩阵*/
// 	R0c[0][1]=T0C[0][1]=0;
// 	R0c[0][2]=T0C[0][2]=0;
// 	R0c[1][0]=T0C[1][0]=0;
// 	R0c[1][1]=T0C[1][1]=1;
// 	R0c[1][2]=T0C[1][2]=0;
// 	R0c[2][0]=T0C[2][0]=0;
// 	R0c[2][1]=T0C[2][1]=0;
// 	R0c[2][2]=T0C[2][2]=1;
// 	T0C[3][0]=0;
// 	T0C[3][1]=0;
// 	T0C[3][2]=0;
// 	
// 	rca0[0]=0; rca0[1]=0; rca0[2]=0;
// 	T0C[0][3]=0; T0C[1][3]=0; T0C[2][3]=0; T0C[3][3]=1;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPathPlanDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pframe;
	pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)pframe->GetActiveView(); //得到当前活动的视窗指针
	doc=view->GetDocument()->doc_delay;                          //得到视窗中物体的指针
	int ID=((CSliderCtrl*)pScrollBar)->GetDlgCtrlID();
	switch(ID)
	{
	case IDC_SLIDER_JNT0:
		m_JNT[0]=((float)((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT0))->GetPos())/1000;
		UpdateData(FALSE);
		doc->m_Module[0].JntVar_trans=m_JNT[0];
		view->InvalidateRect(NULL, FALSE);
		break;
	case IDC_SLIDER_JNT1:
		m_JNT[1]=(float)((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT1))->GetPos();
		UpdateData(FALSE);
		doc->m_Module[1].JntVar_rot=m_JNT[1];
		view->InvalidateRect(NULL, FALSE);
		break;
	case IDC_SLIDER_JNT2:
		m_JNT[2]=(float)((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT2))->GetPos();
		UpdateData(FALSE);
		doc->m_Module[2].JntVar_rot=m_JNT[2];
		view->InvalidateRect(NULL, FALSE);
		break;
	case IDC_SLIDER_JNT3:
		m_JNT[3]=(float)((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT3))->GetPos();
		UpdateData(FALSE);
		doc->m_Module[3].JntVar_rot=m_JNT[3];
		view->InvalidateRect(NULL, FALSE);
		break;
	case IDC_SLIDER_JNT4:
		m_JNT[4]=(float)((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT4))->GetPos();
		UpdateData(FALSE);
		doc->m_Module[4].JntVar_rot=m_JNT[4];
		view->InvalidateRect(NULL, FALSE);
		break;
	case IDC_SLIDER_JNT5:
		m_JNT[5]=(float)((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT5))->GetPos();
		UpdateData(FALSE);
		doc->m_Module[5].JntVar_rot=m_JNT[5];
		view->InvalidateRect(NULL, FALSE);
		break;
	case IDC_SLIDER_JNT6:
		m_JNT[6]=(float)((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT6))->GetPos();
		UpdateData(FALSE);
		doc->m_Module[6].JntVar_rot=m_JNT[6];
		view->InvalidateRect(NULL, FALSE);
		break;
	case IDC_SLIDER_JNT7:
		m_JNT[7]=(float)((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT7))->GetPos();
		UpdateData(FALSE);
		doc->m_Module[7].JntVar_rot=m_JNT[7];
		view->InvalidateRect(NULL, FALSE);
		break;
	default:break;
	}
/*	doc->jnt0=m_JNT0;
	doc->jnt1=m_JNT1;
	doc->jnt2=m_JNT2;
	doc->jnt3=m_JNT3;
	doc->jnt4=m_JNT4;
	doc->jnt5=m_JNT5;
	doc->jnt6=m_JNT6;
	doc->jnt7=m_JNT7;*/
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

}

void CPathPlanDlg::OnButtonSet() 
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT0))->SetPos(int(m_JNT[0]));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT1))->SetPos(int(m_JNT[1]));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT2))->SetPos(int(m_JNT[2]));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT3))->SetPos(int(m_JNT[3]));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT4))->SetPos(int(m_JNT[4]));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT5))->SetPos(int(m_JNT[5]));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT6))->SetPos(int(m_JNT[6]));
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT7))->SetPos(int(m_JNT[7]));

	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();

	multi_doc = view->GetDocument();
	doc = multi_doc->doc_delay;
	doc_real = multi_doc->doc_real;
	
	doc->m_Module[0].JntVar_trans=m_JNT[0];
	for (int i = 0; i < JOINT_NUM; i++) {
		doc->m_Module[i].JntVar_rot = m_JNT[i];
		doc->angelset[i] = m_JNT[i];
		des_angle[i] = m_JNT[i];
	}

	view->InvalidateRect(NULL, FALSE);
	if(ControlMode==1)
	{
		int device=frame->pSiderBar->pCtrlTab->pInitHardware->m_device;
		int PmacStateValue=frame->pSiderBar->pCtrlTab->pInitHardware->PmacstateValue;
		if(PmacStateValue)
		{
			PosCts=(360*30*CODEANGTRANS*(-m_JNT[0]))/(PI*DIAMETER);
			char PosStr[100];
			char outstr[256]="#1j=";  
			_gcvt(PosCts,50,PosStr);
			strcat(outstr,PosStr);
			//	AfxMessageBox(outstr);
			PmacGetResponseA(PMacdevice,buf,255,outstr);
				float p,v;
			PmacGetResponseA(PMacdevice,buf,255,"p");
			p=atof(buf);
			PmacGetResponseA(PMacdevice,buf,255,"v");
			v=atof(buf);
			while(abs(p-PosCts)>50||v!=0)
			{
					PmacGetResponseA(PMacdevice,buf,255,"p");
					p=atof(buf);
					PmacGetResponseA(PMacdevice,buf,255,"v");
					v=atof(buf);

					PmacGetResponseA(PMacdevice,buf,255,"?"); 
        			char b=buf[9];
					if(b=='C')
					{
						b=12;
					}
					if(b&8)
					{

						PmacGetResponseA(PMacdevice,buf,255,"k");
						break;
					}

			}

			PmacGetResponseA(PMacdevice,buf,255,"k");
//			Sleep(60000);
//			PmacGetResponseA(1,buf,255,"k");
	
		}
		SetTimer(3, 100, NULL);
		SetTimer(4, 100, NULL);
		start = clock();
		
		float ff, cons;
		int t = -1;
		for(int i=1;i<8;i++, t*= -1)
		{
			::PCube_resetModule(device,i);
			if (::PCube_getPos(device, i, &ff) == 0) {
				cons = t*ff*180/PI;
				doc_real->m_Module[i].JntVar_rot = cons;
				doc_real->jnt[i] = cons;
				doc_real->angelset[i] = cons;
				
			}
			
			des_vel[i] = 2*PI/180;
			des_acc[i] = 4*PI/180;
		}


		Sleep(3000);
		for (int i = 1, t = -1; i < JOINT_NUM; i++, t *= -1) {
			::PCube_moveRamp(device, i, t*m_JNT[i]*PI/180, 2*PI/180, 4*PI/180);
		}

		/*
		::PCube_moveRamp(device,1,-m_JNT[1]*PI/180,2*PI/180,4*PI/180);
		//Sleep(2000);
		::PCube_moveRamp(device,2,m_JNT[2]*PI/180,2*PI/180,4*PI/180);
		//Sleep(2000);
		::PCube_moveRamp(device,3,-m_JNT[3]*PI/180,2*PI/180,4*PI/180);
		//Sleep(2000);
		::PCube_moveRamp(device,4,m_JNT[4]*PI/180,2*PI/180,4*PI/180);
		//Sleep(2000);
		::PCube_moveRamp(device,5,-m_JNT[5]*PI/180,2*PI/180,4*PI/180);
		//Sleep(2000);
		::PCube_moveRamp(device,6,m_JNT[6]*PI/180,2*PI/180,4*PI/180);
		//sSleep(2000);
		::PCube_moveRamp(device,7,-m_JNT[7]*PI/180,2*PI/180,4*PI/180);
		*/

//		Sleep(4000);
//		PmacGetResponseA(0,buf,255,"k");


	}
}

void CPathPlanDlg::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	m_JNT[0]=0;
	m_JNT[1]=-7.5;
	m_JNT[2]=0;
	m_JNT[3]=0;
	m_JNT[4]=0;
	m_JNT[5]=0;
	m_JNT[6]=0;
	m_JNT[7]=0;
	UpdateData(false);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT0))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT1))->SetPos(-7.5);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT2))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT3))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT4))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT5))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT6))->SetPos(0);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_JNT7))->SetPos(0);

	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	
	multi_doc = view->GetDocument();
	doc = multi_doc->doc_delay;
	doc_real = multi_doc->doc_real;

	doc_real->m_Module[0].JntVar_trans = doc->m_Module[0].JntVar_trans = m_JNT[0];
	for (int i = 0; i < JOINT_NUM; i++) {
		doc_real->jnt[i] = doc->jnt[i] = m_JNT[i];
		doc_real->m_Module[i].JntVar_rot = doc->m_Module[i].JntVar_rot = m_JNT[i];
		doc->angelset[i] = doc->angelset[i] = m_JNT[i];
	}
	
	
	view->InvalidateRect(NULL, FALSE);
	if(ControlMode==1)
	{
		int device=frame->pSiderBar->pCtrlTab->pInitHardware->m_device;
		int PmacStateValue=frame->pSiderBar->pCtrlTab->pInitHardware->PmacstateValue;
		if(PmacStateValue)
		{
//			PmacGetResponseA(1,buf,255,"#1HM");//改为调用回零按钮
			frame->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_ZERO)->SendMessage(BM_CLICK,1,1);
		}

		for(int i=7;i>0;i--)
		{
			::PCube_moveRamp(device,
				i,
				0,
				2*PI/180,
				4*PI/180);
			Sleep(2000);
		}
//		::PCube_moveRamp(device,1,-7.5*PI/180 ,4*PI/180,16*PI/180);
//		Sleep(200);
//		PmacGetResponseA(0,buf,255,"k");
	}
}

void CPathPlanDlg::OnButtonInit() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	((CButton*)GetDlgItem(IDC_RADIO_LINEARPLAN))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_RADIO_AUTOPLAN))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_RADIO_PROGRAMM))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_RADIO_HANDCONTROL))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_RUN))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_STOP))->EnableWindow(true);

	double PEI[6];
	double basepe[6]={m_baspx,m_baspy,m_baspz,m_basaif*PI/180,m_basbit*PI/180,m_basgam*PI/180};
	////初始关节角
	int i = 0;
	ini_ang[0]=m_JNT[0];
	for (i = 1;  i < JOINT_NUM; i++) {
		ini_ang[i] = m_JNT[i]*PI/180;
	}
 
	Forwardkine_static(ini_ang, PEI ); //得到初始末端位姿
  
    for(int i=0; i<6; i++)  { if(fabs(PEI[i])<10e-6) PEI[i]=0;   }
	
    UpdateData(false);		
	//将初始末端位姿赋给DOC类
	doc->ini_px=PEI[0];
	doc->ini_py=PEI[1];
	doc->ini_pz=PEI[2];
	
    doc->ini_aif=PEI[3]*180/PI;
	doc->ini_bit=PEI[4]*180/PI;
	doc->ini_gam=PEI[5]*180/PI;
	doc->No=No=0;
	doc->numtime=numtime=0;
	doc->Lineartestflag=false;
	PreProgrammFlag=0;
	UpdateData(true);
	//初始化参数
	if (ControlMode==1)
	{
		PmacGetResponseA(PMacdevice,buf,255,"j/");
		PmacGetResponseA(PMacdevice,buf,255,"&1");
		PmacGetResponseA(PMacdevice,buf,255,"CLOSE");
		PmacGetResponseA(PMacdevice,buf,255,"#1->X");	
		PmacGetResponseA(PMacdevice,buf,255,"&1 DEFINE ROT 200");
		PmacGetResponseA(PMacdevice,buf,255,"OPEN ROT");
		PmacGetResponseA(PMacdevice,buf,255,"CLEAR");
		PmacGetResponseA(PMacdevice,buf,255,"LINEAR");
		PmacGetResponseA(PMacdevice,buf,255,"ABS");
		PmacGetResponseA(PMacdevice,buf,255,"TA10");
		PmacGetResponseA(PMacdevice,buf,255,"TS0");
		PmacGetResponseA(PMacdevice,buf,255,"F150000");
		PmacGetResponseA(PMacdevice,buf,255,"CLOSE");
	}
	view->InvalidateRect(NULL, FALSE);
	
}



int CPathPlanDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	pLinerPlanSetDlg=new CLinerPlanSetDlg();
	pLinerPlanSetDlg->Create(IDD_LINERPLANSETTING,this);
	pAutoPlanSetDlg=new CAutoPlanSetDlg();
	pAutoPlanSetDlg->Create(IDD_AUTOPLANSETTING,this);
	pHandControlDlg=new CHandControlDlg();
	pHandControlDlg->Create(IDD_HANDCONTROL,this);
	
	return 0;
}

void CPathPlanDlg::OnRadioLinearplan() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_LINEARPLAN))->SetCheck(BST_CHECKED);
	Planradio=IDC_RADIO_LINEARPLAN;
	pLinerPlanSetDlg->MoveWindow(350, 150, 300, 260);
	//pLinerPlanSetDlg->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_BUTTON_INIOK))->EnableWindow(true);
	///////
//	COMPlanradio=0x00;
	
}

void CPathPlanDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int SingularFlag=0;
	double wcm[3]={0, 0, 0};
	double now_baspe[6],now_basvel[6], now_angvel[8];
    int i,j;
	CString str;

	if(nIDEvent==0)
	{
		////////////////函数调用部分
		for(i=0; i<8; i++)
		{
			now_ang[i]=next_ang[i];
			now_angvel[i]=next_angvel[i];
		}
		for(i=0; i<6; i++)
		{
			now_baspe[i]=next_baspe[i];
			now_basvel[i]=next_basvel[i];
		}
		if (COMPILE) {
			for (i = 0; i < 6; i++) {
				_cprintf("next_ang(%d): %f\n", i, next_ang[i]);
			}
		}
		LinearMotionplan(now_ang, now_baspe, PEint, PEend, m_time, m_ts, No+1, 0, next_baspe, next_basvel, next_ang, next_angvel);
		if (SingularFlag==1)
		{
			KillTimer(0);
			AfxMessageBox("运动学奇异");
		}
		qmtomModule();
		ShowJntVariable();
		if (next_ang[1]<-PI||next_ang[1]>PI)
		{
			KillTimer(0);
			AfxMessageBox("关节1越界");
		}
		if (next_ang[2]<-PI/2||next_ang[2]>PI/2)
		{
			KillTimer(0);
			AfxMessageBox("关节2越界");
		}
		if (next_ang[3]<-PI||next_ang[3]>PI)
		{
			KillTimer(0);
			AfxMessageBox("关节3越界");
		}
		if (next_ang[4]<-2*PI/3||next_ang[4]>2*PI/3)
		{
			KillTimer(0);
			AfxMessageBox("关节4越界");
		}
		if (next_ang[5]<-PI||next_ang[5]>PI)
		{
			KillTimer(0);
			AfxMessageBox("关节5越界");
		}
		if (next_ang[6]<-5*PI/6||next_ang[6]>5*PI/6)
		{
			KillTimer(0);
			AfxMessageBox("关节6越界");
		}
		if ( next_ang[7]<-PI || next_ang[7]>PI )
		{
			KillTimer(0);
			AfxMessageBox("关节7越界");
		}

		for(i=0; i<8; i++)
		{
			ini_ang[i] = next_ang[i];
		}
		time_t cur_time = clock();
		int prc_time = cur_time - start_linear;
		if (btn_release && prc_time > 500) {
			if (COMPILE) {
				_cprintf("The button has released.\n");
			}
			KillTimer(0);
		}
		No++;
		numtime++;
		for(i=0;i<8;i++)
		{	
	   		ceta[No+1][i]=next_ang[i];//保存数据用
		}
		if (ControlMode == 1)
		{
			SendMessageToJXB();
		}

		//////////////终止判断及画图数据操作
		if(No>kn-1) 
		{   
			KillTimer(0);
			if (COMPILE) {

				_cprintf("No is: %d\n", No);
				for (int i = 0; i < No; i++) {
					for (int j = 0; j < 8; j++) {
						_cprintf("ceta(%d, %d) is: %f\t", i, j, ceta[i][j]);
					}
					_cprintf("\n");
				}
			}
		}
	}
	if(nIDEvent==1)
	{

		for(i=0; i<8; i++)
		{
			now_ang[i]=next_ang[i];
			now_angvel[i]=next_angvel[i];
		}
		for(i=0; i<6; i++)
		{
			now_baspe[i]=next_baspe[i];
			now_basvel[i]=next_basvel[i];
		}
//		if (HandEyeFlag==1)
//		{
//			Auto_PEdesUpdate(); 
//		}
        CaptureCommand=AutoMotionPlan_line(now_ang, now_baspe, GivenObj, wcm, VelocityLimit, CapOrientFlag, No+1,0,next_baspe, next_basvel, next_ang, next_angvel);
		if (SingularFlag==1)
		{
			KillTimer(1);
			AfxMessageBox("运动学奇异");
		}
		if (next_ang[1]<-PI||next_ang[1]>PI)
		{
			KillTimer(1);
			AfxMessageBox("关节1越界");
		}
		if (next_ang[2]<-PI/2||next_ang[2]>PI/2)
		{
			KillTimer(1);
			AfxMessageBox("关节2越界");
		}
		if (next_ang[3]<-PI||next_ang[3]>PI)
		{
			KillTimer(1);
			AfxMessageBox("关节3越界");
		}
		if (next_ang[4]<-2*PI/3||next_ang[4]>2*PI/3)
		{
			KillTimer(1);
			AfxMessageBox("关节4越界");
		}
		if (next_ang[5]<-PI||next_ang[5]>PI)
		{
			KillTimer(1);
			AfxMessageBox("关节5越界");
		}
		if (next_ang[6]<-5*PI/6||next_ang[6]>5*PI/6)
		{
			KillTimer(1);
			AfxMessageBox("关节6越界");
		}
		if (next_ang[7]<-PI||next_ang[7]>PI)
		{
			KillTimer(1);
			AfxMessageBox("关节7越界");
		}
		qmtomModule();
		ShowJntVariable();
		No++;
		numtime++;
		for(i=0;i<8;i++)
		{	
			ceta[No+1][i]=next_ang[i];//保存数据用
		}
		if (ControlMode==1)
		{
			SendMessageToJXB();
		}
       //////////////终止判断及画图数据操作
		//////函数终止判断
        if(CaptureCommand==1 || No>Ni-1) 
		{   
			KillTimer(1);  
			kn=No;
		}
	}
	if(nIDEvent==2)
	{
		doc->m_Module[0].JntVar_trans=ceta[No][0];
		for (i=1;i<8;i++)
		{
			doc->m_Module[i].JntVar_rot=ceta[No][i]*180/PI;
		}
		UpdateData(false);

		if (ControlMode==1)
		{
			CMainFrame *pframe;
			pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
			int device=pframe->pSiderBar->pCtrlTab->pInitHardware->m_device;
			double send_ang[8];
			send_ang[1]=-ceta[No][1];
			send_ang[2]=ceta[No][2];
			send_ang[3]=-ceta[No][3];
			send_ang[4]=ceta[No][4];
			send_ang[5]=-ceta[No][5];
			send_ang[6]=ceta[No][6];
			send_ang[7]=-ceta[No][7];
			for(int i=1;i<8;i++)
			{
				::PCube_moveRamp(device,
					i,
					send_ang[i],
					4*PI/180,
					16*PI/180);
			}
		}
			No++;
			numtime++;
			if (No>kn-1)
			{
				KillTimer(2);
			}
		
	}

	// added by gjx
	if (nIDEvent == 3)
	{
		CMainFrame *pframe;
		pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
		
		int device=pframe->pSiderBar->pCtrlTab->pInitHardware->m_device;
		float ff;

		//int test_1 = ::PCube_getPos(device, 1, &ff);
		//_cprintf("ff*180/PI is: %f\n", ff);
		
		float cons;
		int t = 1;
		for (int i = 0; i < JOINT_NUM; i++, t *= -1) {
			if (::PCube_getPos(device, i, &ff) == 0)
			{
				cons = ff*180/PI*t;
				doc->m_Module[i].JntVar_rot = cons;
				doc->jnt[i] = cons;
				doc->angelset[i] = cons;
				m_JNT[i] = cons;
			}
		}
		end = clock();
		
		if (end - start > 30000 && deviceIsHalt())
		{
			KillTimer(3);
		}
		UpdateData(FALSE);
		view->InvalidateRect(NULL, FALSE);
	}
	if (nIDEvent == 4)
	{	
		clock_t cur_time = clock();
		if (COMPILE) {
			_cprintf("current time is %d\n", cur_time);
		}
		int delay = cur_time - start;
		if (COMPILE) {
			_cprintf("delay is: %d\n", delay);
		}
		if (cur_time - start > this->predict_time*1000) {
			
			double cons = 0.2*2;
			
			for (int i = 1; i < 8; i++) {
				int t = isNear(doc_real->m_Module[i].JntVar_rot, des_angle[i]);
				if (COMPILE) {
					_cprintf("for joint %d, jnt value is: %f, des_angle is: %f, and t is: %d\n",
						i, doc_real->m_Module[i].JntVar_rot, des_angle[i], t);
				}
				if (t != 0) { 
					doc_real->m_Module[i].JntVar_rot += cons*t;
					doc_real->jnt[i] += cons*t;
					doc_real->angelset[i] += cons*t;
					_cprintf("current angle for joint %d: %f\n", i, doc_real->jnt[i]);
					//break;
				}
			}
			if (COMPILE)
				system("pause");
		}

		end = clock();
		if (end - start > 30000 && simFinishMoving())
		{
			KillTimer(4);
		}
		
		UpdateData(FALSE);
		view->InvalidateRect(NULL, FALSE);
	}
	view->InvalidateRect(NULL, FALSE);
	CDialog::OnTimer(nIDEvent);
}

int CPathPlanDlg::isNear(double a, double b) {
	if (abs(b - a) < 0.21) {
		return 0;
	} else if (a < b) {
		return 1;
	}
	return -1;
}

bool CPathPlanDlg::simFinishMoving() {
	int t = -1;
	for (int i = 1; i < JOINT_NUM; i++, t *= -1) {
		if (!isNear(doc_real->m_Module[i].JntVar_rot, t*des_angle[i])) return false;
	}
	return true;
}

bool CPathPlanDlg::deviceIsHalt()
{
	CMainFrame *pframe;
	pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
	int device=pframe->pSiderBar->pCtrlTab->pInitHardware->m_device;
	float vel;
	
	float min_speed = 0.000001;
	::PCube_getVel(device, 1, &vel);
	if (min_speed < abs(vel))
		return false;
	::PCube_getVel(device, 2, &vel);
	if (min_speed < abs(vel))
		return false;
	::PCube_getVel(device, 3, &vel);
	if (min_speed < abs(vel))
		return false;
	::PCube_getVel(device, 4, &vel);
	if (min_speed < abs(vel))
		return false;
	::PCube_getVel(device, 5, &vel);
	if (min_speed < abs(vel))
		return false;
	::PCube_getVel(device, 6, &vel);
	if (min_speed < abs(vel))
		return false;
	::PCube_getVel(device, 7, &vel);
	if (min_speed < abs(vel))
		return false;
	return true;
}

void CPathPlanDlg::qmtomModule() 
{
	doc->m_Module[0].JntVar_trans=next_ang[0];
	doc->jnt[0]=next_ang[0];
	for (int i = 1; i < 8; i++) {
		int cons = next_ang[i]*180/PI;
		doc->m_Module[i].JntVar_rot = cons;
		doc->jnt[i] = cons;
		this->m_JNT[i] = cons;
	}
	doc->No=No;
	doc->numtime=numtime;
	UpdateData(false); 
}

void CPathPlanDlg::ShowJntVariable()
{
	int i;	
	double PEI[6], petemp[3];
	Forwardkine_static(now_ang, PEI ); 
	
	////显示数据传递
	for(i=0; i<3; i++)  
	{
		doc->EndPE[i] = EndPE[No][i] = PEI[i];   //末端位姿
		doc->EndPE[i+3] = EndPE[No][i+3] = PEI[i+3]*180/PI; 
//		doc->EndVW[i]=Ve[No][i];   //末端速度
//		doc->EndVW[i+3]=We[No][i]*180/PI;
	}

	for(i=0; i<8; i++)
	{
		doc->JntW[i] = next_angvel[i]*180/PI;  //关节角速度
//		doc->JntTau[i]= FbTau[No][i+6];
	}
		
	UpdateData(false);
}

void CPathPlanDlg::OnButtonIniok() 
{
	// TODO: Add your control notification handler code here
	double Tmd[4][4];
    UpdateData(true);	

	// added by gjx, to modify the ini_ang
	/*ini_ang[0] = 0.0f;
	for (int i = 1; i < 8; i++)  {
		ini_ang[i] = this->m_JNT[i]*PI/180;
	}*/

	Forwardkine_static(ini_ang, PEint);
	
	if(Planradio==IDC_RADIO_LINEARPLAN )
	{	
		/*m_dpx=doc->des_px;
		m_dpy=doc->des_py;
		m_dpz=doc->des_pz;
		m_daif=doc->des_aif;
		m_dbit=doc->des_bit;
		m_dgam=doc->des_gam;

		PEend[0]=m_dpx;  PEend[1]=m_dpy;  PEend[2]=m_dpz;
		PEend[3]=m_daif*PI/180;  PEend[4]=m_dbit*PI/180;  PEend[5]=m_dgam*PI/180;*/

		for (int i = 0; i < JOINT_NUM; i++) {
			des_angle[i] = this->m_JNT[i]*PI/180;
		}
		Forwardkine_static(des_angle, PEend);
		doc->des_px = PEend[0];
		doc->des_py = PEend[1];
		doc->des_pz = PEend[2];
		doc->des_aif = PEend[3];
		doc->des_bit = PEend[4];
		doc->des_gam = PEend[5];

		doc->Lineartestflag=true;
		UpdateData(FALSE);
	}

	if(Planradio==IDC_RADIO_AUTOPLAN)
	{
//		AutoPlanObjFlag=doc->AutoPlanObjFlag;
		CapOrientFlag=doc->CapOrientFlag;
/*		HandEyeFlag=pAutoPlanSetDlg->HandEyeFlag;
		if (HandEyeFlag==1)
		{
			int i;
			CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
			view=(CRobotControlView*)frame->GetActiveView();
			doc=view->GetDocument(); 
			double plan_PEend_des[6], plan_Tmd[4][4];
			plan_PEend_des[0]=(float)(frame->cam_xd);
			plan_PEend_des[1]=(float)(frame->cam_yd);
			plan_PEend_des[2]=1.588;//(float)(frame->cam_zd)
			plan_PEend_des[3]=frame->cam_theta1d;
			plan_PEend_des[4]=frame->cam_theta2d;
			plan_PEend_des[5]=frame->cam_theta3d;
			for (i=0; i<3; i++)
			{
				doc->EndPE[i]=plan_PEend_des[i];
				doc->EndPE[i+3]=plan_PEend_des[i+3]*RAD_TO_DEG;
			}
			doc->des_px=plan_PEend_des[0];
			doc->des_py=plan_PEend_des[1];
			doc->des_pz=plan_PEend_des[2];
			doc->des_aif=plan_PEend_des[3]*RAD_TO_DEG;
			doc->des_bit=plan_PEend_des[4]*RAD_TO_DEG;
			doc->des_gam=plan_PEend_des[5]*RAD_TO_DEG;
			for (i=0; i<6; i++)
			{
				GivenObj[i]=plan_PEend_des[i];
			}
			PE2T(plan_PEend_des, plan_Tmd);
			for(i=0; i<3; i++)
				doc->PreObj[i]=plan_Tmd[i][3]-ds1*plan_Tmd[i][2];
			PEend_des_ori[0]=plan_PEend_des[0];
			PEend_des_ori[1]=plan_PEend_des[1];
			PEend_des_ori[2]=plan_PEend_des[2];
			PEend_des_ori[3]=plan_PEend_des[3];
			PEend_des_ori[4]=plan_PEend_des[4];
			PEend_des_ori[5]=plan_PEend_des[5];
			
		}*/
		ds1=0.1;
		m_dpx=doc->des_px;
		m_dpy=doc->des_py;
		m_dpz=doc->des_pz;
		m_daif=doc->des_aif;
		m_dbit=doc->des_bit;
		m_dgam=doc->des_gam;
		GivenObj[0]=m_dpx;  GivenObj[1]=m_dpy; 	GivenObj[2]=m_dpz; 
		GivenObj[3]=m_daif*PI/180; GivenObj[4]=m_dbit*PI/180;  GivenObj[5]=m_dgam*PI/180;
		PE2T(GivenObj, Tmd);
			if(CapOrientFlag<4)
			{
				PreObj[0]=Tmd[0][3]-ds1*Tmd[0][CapOrientFlag-1];
				PreObj[1]=Tmd[1][3]-ds1*Tmd[1][CapOrientFlag-1];
				PreObj[2]=Tmd[2][3]-ds1*Tmd[2][CapOrientFlag-1];
			}
			else
			{
				PreObj[0]=Tmd[0][3]+ds1*Tmd[0][CapOrientFlag-4];
				PreObj[1]=Tmd[1][3]+ds1*Tmd[1][CapOrientFlag-4];
				PreObj[2]=Tmd[2][3]+ds1*Tmd[2][CapOrientFlag-4];
			}
			doc->PreObj[0]=PreObj[0];
			doc->PreObj[1]=PreObj[1];
			doc->PreObj[2]=PreObj[2];
			doc->AutoPlanFlag=true;

//		doc->AutoPlanFlag=true;

		UpdateData(FALSE);
	}	
	kn=m_time/t0;
/*	if(ControlMode==1)
	{
		PmacGetResponseA(PMacdevice,buf,255,"j/");
		PmacGetResponseA(PMacdevice,buf,255,"&1");
		PmacGetResponseA(PMacdevice,buf,255,"CLOSE");
		PmacGetResponseA(PMacdevice,buf,255,"#1->X");
		PmacGetResponseA(PMacdevice,buf,255,"OPEN PROG1");
		PmacGetResponseA(PMacdevice,buf,255,"CLEAR");
		PmacGetResponseA(PMacdevice,buf,255,"LINEAR");
		PmacGetResponseA(PMacdevice,buf,255,"ABS");
		PmacGetResponseA(PMacdevice,buf,255,"TA50");
		PmacGetResponseA(PMacdevice,buf,255,"TS0");
	}*/
	view->InvalidateRect(NULL, FALSE);
}

void CPathPlanDlg::OnButtonRun() 
{
	// TODO: Add your control notification handler code here
	double wcm[3]={0,0,0};

	
	if( Planradio==IDC_RADIO_LINEARPLAN)
	{
		LinearMotionplan(ini_ang, basiniPE, PEint, PEend, m_time, m_ts, No, 0, next_baspe, next_basvel, next_ang, next_angvel);

		for(int i=0;i<8;i++)
		{	
			ceta[0][i]=ini_ang[i];
			ceta[1][i]=next_ang[i];//保存数据用
		 }
		SetTimer(0,100,NULL);

	}
	if( Planradio==IDC_RADIO_AUTOPLAN) 
	{
		CaptureCommand=AutoMotionPlan_line(ini_ang, basiniPE, GivenObj, wcm, VelocityLimit, CapOrientFlag, No+1,0,next_baspe, next_basvel, next_ang, next_angvel);
		for(int i=0;i<8;i++)
		{	
			ceta[0][i]=ini_ang[i];
			ceta[1][i]=next_ang[i];//保存数据用
		 }
		SetTimer(1,100,NULL);
    }
	if (Planradio==IDC_RADIO_PROGRAMM)
	{
//		No=0;
//		numtime=0;
		UpdateData(false); 
		SetTimer(2,50,NULL);

	}

	
}

void CPathPlanDlg::OnRadioAutoplan() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_AUTOPLAN))->SetCheck(BST_CHECKED);
    Planradio=IDC_RADIO_AUTOPLAN;
	pAutoPlanSetDlg->MoveWindow(350, 150, 330, 350);
	pAutoPlanSetDlg->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_BUTTON_INIOK))->EnableWindow(true);
	
}

void CPathPlanDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CMainFrame *pframe;
	pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
	int device=pframe->pSiderBar->pCtrlTab->pInitHardware->m_device;
	if(GetDlgItemText(IDC_BUTTON_STOP,str),str=="暂 停")
    {	
		if(Planradio==IDC_RADIO_AUTOPLAN)
		{
			KillTimer(1);
		}
		else if(Planradio==IDC_RADIO_LINEARPLAN)
		{
			KillTimer(0);
		}
		else
		{
			KillTimer(2);
		}
		if (ControlMode==1)
		{	
//			PmacSetIVariableDouble(0,122,0);
			PmacGetResponseA(PMacdevice,buf,255,"q");
			PmacGetResponseA(PMacdevice,buf,255,"k");
			for (int i=1;i<8;i++)
			{	
				PCube_setRampVel(device,i,1);
			}
		}
		SetDlgItemText(IDC_BUTTON_STOP,"恢 复");
	}
	else 
	{  
		if(Planradio==IDC_RADIO_AUTOPLAN)
		{
			SetTimer(1,100,NULL);
		}
		else if(Planradio==IDC_RADIO_LINEARPLAN)
		{
			SetTimer(0,100,NULL);
		}
		else
		{
			SetTimer(2,100,NULL);
		}
		if (ControlMode==1)
		{
//			PmacSetIVariableDouble(0,122,32);
			PmacGetResponseA(PMacdevice,buf,255,"&1 R");
			for (int i=1;i<8;i++)
			{
				PCube_setRampVel(device,i,2*PI/180);
			}
		}
		SetDlgItemText(IDC_BUTTON_STOP,"暂 停");}
	}


void CPathPlanDlg::OnRadioHandcontrol() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_HANDCONTROL))->SetCheck(BST_CHECKED);
    Planradio=IDC_RADIO_HANDCONTROL;
	pHandControlDlg->MoveWindow(750, 150, 330, 410);
	pHandControlDlg->ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_BUTTON_INIOK))->EnableWindow(true);
}

void CPathPlanDlg::OnRadioSumu() 
{
	// TODO: Add your control notification handler code here
	ControlMode=0;
	
}

void CPathPlanDlg::OnRadioJxb() 
{
	// TODO: Add your control notification handler code here
	ControlMode=1;
	
}

void CPathPlanDlg::SendMessageToJXB()
{	
	CMainFrame *pframe;
	pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
	int device=pframe->pSiderBar->pCtrlTab->pInitHardware->m_device;
	double send_ang[8];
	send_ang[0]=-next_ang[0];
	send_ang[1]=-next_ang[1];
	send_ang[2]=next_ang[2];
	send_ang[3]=-next_ang[3];
	send_ang[4]=next_ang[4];
	send_ang[5]=-next_ang[5];
	send_ang[6]=next_ang[6];
	send_ang[7]=-next_ang[7];
	for(int i=1;i<8;i++)
	{
		/*::PCube_moveRamp(device,
				i,
				send_ang[i],
				1*PI/180,
				1*PI/180);*/
			::PCube_moveStep(device,
				i,
				send_ang[i],
				4000);
	}
/////////////////////导轨控制//////////////////////////////
//	VelCts=m_velocity*61440/270;
//	PosCts=m_position*61440/270;
/*	VelCts=20;
	PosCts=(360*30*CODEANGTRANS*send_ang[0])/(PI*DIAMETER);
	PmacSetIVariableDouble(0,122,VelCts);
	char PosStr[100];
	char outstr[256]="#1j=";  //绝对运动指令，先电机回零
	_gcvt(PosCts,50,PosStr);
	strcat(outstr,PosStr);
	//	AfxMessageBox(outstr);
	PmacGetResponseA(PMacdevice,buf,255,outstr);    */ 

//	char strbita[50];
//	char outstr2[100]="X";
//	char stralpha[50];
//	char outstr1[100]="F";


/*	if (int(No/10)==No/10)
	{
		PosCts=(360*30*CODEANGTRANS*send_ang[0])/(PI*DIAMETER);
		_gcvt(PosCts,45,strbita);
		strcat(outstr2,strbita);
		//	VelCts=(360*30*CODEANGTRANS*100)/(PI*DIAMETER*1000);
		//	_gcvt(VelCts,45,stralpha);
		//	strcat(outstr1,stralpha);


		PmacGetResponseA(PMacdevice,buf,255,"OPEN ROT");
		PmacGetResponseA(PMacdevice,buf,255,"CLEAR");
		//	PmacGetResponseA(PMacdevice,buf,255,"LINEAR");
		//	PmacGetResponseA(PMacdevice,buf,255,outstr1);
		PmacGetResponseA(PMacdevice,buf,255,"F150000");
		PmacGetResponseA(PMacdevice,buf,255,outstr2);
		PmacGetResponseA(PMacdevice,buf,255,"CLOSE");
		PmacGetResponseA(PMacdevice,buf,255,"&1 R");
	}*/
	

/*	float p,v;
	PmacGetResponseA(PMacdevice,buf,255,"p");
	p=atof(buf);
	PmacGetResponseA(PMacdevice,buf,255,"v");
	v=atof(buf);
	while(abs(p-PosCts)>50||v!=0)
	{
			PmacGetResponseA(PMacdevice,buf,255,"p");
			p=atof(buf);
			PmacGetResponseA(PMacdevice,buf,255,"v");
			v=atof(buf);

			PmacGetResponseA(PMacdevice,buf,255,"?"); 
        	char b=buf[9];
			if(b=='C')
			{
				b=12;
			}
	        if(b&8)
			{

				PmacGetResponseA(PMacdevice,buf,255,"k");
				break;
			}

	}

			PmacGetResponseA(PMacdevice,buf,255,"k");*/
}

void CPathPlanDlg::OnButtonSavedata() 
{
	// TODO: Add your control notification handler code here
	int i;
	FILE *fp;
	CString filename; 
	CFileDialog dlg(false);
	if(dlg.DoModal()==IDOK) 
		filename=dlg.GetFileName();
	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
 //   doc->filename=filename; 

	fp=fopen(filename+"预编程关节基座数据.txt","w");
	if(fp==NULL)	exit(1);
	else
	{
		fprintf(fp,"%d\n",kn);
		for(i=0; i<kn; i++)
			fprintf(fp,"%f %f %f %f %f %f %f %f\n",ceta[i][0],ceta[i][1],ceta[i][2],ceta[i][3],ceta[i][4],ceta[i][5],ceta[i][6],ceta[i][7]);
	}
	fclose(fp); 
	
}

void CPathPlanDlg::OnRadioProgramm() 
{
	// TODO: Add your control notification handler code here
	CString t1="NULL";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"txt File(*.txt)|*.txt||All Files (*.*)|*.*||",NULL);
	if(dlg.DoModal()==IDOK)
		t1=dlg.GetFileName();
	int i,j;  
    float theta[7];
	double PEI[6];
	//////////////////////////////////////////////
    FILE *inputfile;
	if(t1=="NULL") t1="NULLNAME.txt";
	inputfile=fopen(t1,"r");
	if(t1=="NULLNAME.txt")
	{	
		//exit(1);
		MessageBox("请输入一个文件名!");
		//		fclose(inputfile1); 
	}
	else
	{
		fscanf(inputfile,"%d\n",&kn);
		for(i=0; i<kn; i++)
		{
			fscanf(inputfile,"%f %f %f %f %f %f %f %f\n",&theta[0],&theta[1],&theta[2],&theta[3],&theta[4],&theta[5],&theta[6],&theta[7]);
			for(j=0; j<8; j++) 
			{
				ceta[i][j]=theta[j]; 
			}
		}
		fclose(inputfile); 
	}
	No=0;
	numtime=0;
	Planradio=IDC_RADIO_PROGRAMM;

}


void CPathPlanDlg::OnButtonKeyControl()
{
	ctrl_mode = 1;
	// use joyGetPos method to justify the current state of device
	JOYINFO* pji = new JOYINFO;
	int res = joyGetPos(JOYSTICKID1, pji);
	int result = joySetCapture(m_hWnd, JOYSTICKID1, 0, FALSE);
	if (result == JOYERR_NOCANDO && res != JOYERR_NOERROR) {
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox("不能捕获游戏杆", NULL, MB_OK | MB_ICONEXCLAMATION);
	}
	if (result == JOYERR_UNPLUGGED && res != JOYERR_NOERROR) {
		MessageBeep(MB_ICONEXCLAMATION);
		MessageBox("游戏杆未与系统连接", NULL, MB_OK | MB_ICONEXCLAMATION);
	}
}

void CPathPlanDlg::linearMove(int direction) {
	if (COMPILE) {
		_cprintf("you call the linearMove with direction: %d\n", direction);
	}
	UpdateData(true);	
	Forwardkine_static(ini_ang, PEint);
	int i = 0; 
	double cons = 0.5;
	for (; i < 6; i++) {
		PEend[i] = PEint[i];
	}
	switch (direction) {
	case 0:
		PEend[0] -= cons;
		PEend[1] -= cons;
		PEend[2] -= cons;
		break;
	case 1:
		PEend[0] += cons;
		PEend[2] -= cons;
		break;
	case 2:
		PEend[0] += cons;
		PEend[1] -= 2*cons;
		PEend[2] += cons;
		break;
	case 3:
		PEend[0] += cons;
		PEend[1] += cons;
		PEend[2] += cons;
		break;
	case 4:
		PEend[0] -= cons;
		PEend[2] += cons;
		break;
	case 5:
		PEend[0] -= cons;
		PEend[1] += 2*cons;
		PEend[2] -= cons;
		break;
	default:
		break;
	}

	doc->des_px = PEend[0];
	doc->des_py = PEend[1];
	doc->des_pz = PEend[2];
	doc->des_aif = PEend[3];
	doc->des_bit = PEend[4];
	doc->des_gam = PEend[5];

	doc->Lineartestflag=true;
	UpdateData(FALSE);

	kn = m_time/t0;
	view->InvalidateRect(NULL, FALSE);

	LinearMotionplan(ini_ang, basiniPE, PEint, PEend, m_time, m_ts, No, 0, next_baspe, next_basvel, next_ang, next_angvel);

	for(int i=0;i<8;i++)
	{	
		ceta[0][i]=ini_ang[i];
		ceta[1][i]=next_ang[i];//保存数据用
	}
	SetTimer(0,100,NULL);
	start_linear = clock();
}

void CPathPlanDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	int state = (nFlags >> 14) & 1;
	if (state == 1) return;

	btn_release = false;
	if (ctrl_mode == 1) {
		switch (nChar) {
			case 'A':
				linearMove(0);
				break;
			case 'S':
				linearMove(1);
				break;
			case 'D':
				linearMove(2);
				break;
			case 'Q':
				linearMove(3);
				break;
			case 'W':
				linearMove(4);
				break;
			case 'E':
				linearMove(5);
				break;
			default:
				break;
		}
	}
}

void CPathPlanDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	btn_release = true;
	switch (nChar) {
		case 'A':
			break;
		case 'S':
			break;
		case 'D':
			break;
		case 'Q':
			break;
		case 'W':
			break;
		case 'R':
			break;
		default:
			break;
	}
}

LRESULT CPathPlanDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	if (ctrl_mode == 1) {
		switch(message){	
		case MM_JOY1BUTTONDOWN :
			btn_release = false;
			if (wParam & JOY_BUTTON1){
				linearMove(0);
			}
			if (wParam & JOY_BUTTON2){
				linearMove(1);

			}
			if (wParam & JOY_BUTTON3){
				linearMove(2);
			}
			if (wParam & JOY_BUTTON4){
				linearMove(3);
			}	
			break;
		case MM_JOY1BUTTONUP :
			btn_release = true;
			break;
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}	

void CPathPlanDlg::viewProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	btn_release = false;
	if (ctrl_mode == 1) {
		switch(message){	
		case MM_JOY1BUTTONDOWN :
			if (wParam & JOY_BUTTON1){
				linearMove(0);
			}
			if (wParam & JOY_BUTTON2){
				linearMove(1);

			}
			if (wParam & JOY_BUTTON3){
				linearMove(2);
			}
			if (wParam & JOY_BUTTON4){
				linearMove(3);
			}	
			break;
		case MM_JOY1BUTTONUP :
			btn_release = true;
			break;
		}
	}
}



void CPathPlanDlg::OnButtonCamera()
{
	// TODO: 在此添加控件通知处理程序代码
	XnStatus eResult;
	Context mContext;
	xn::ImageGenerator mImageGenerator; 
	xn::DepthGenerator mDepthGenerator; 
	const XnDepthPixel * depthMD;
	const XnUInt8 * imgMD ;
	xn::DepthMetaData mDepthMD;


	IplImage * depthrgbimg;
	IplImage * dep16Img ;
	IplImage * bgrImg ;
	IplImage * depthImg ;
	IplImage * depthImg1 ;
	IplImage * rgbImg ;
	IplImage *image;

	bool StartBtnFlag;
	char txtfile[100];
	char savefile[100];

	XnPoint3D selectedPoint;
	FILE* endposition;

	eResult = mContext.Init();  
	//CheckOpenNIError( eResult, "initialize context" );  

	// 3. create depth generator  
	eResult = mDepthGenerator.Create( mContext ); 
	//CheckOpenNIError( eResult, "Create depth generator" ); 

	// 4. create image generator  	
	eResult = mImageGenerator.Create( mContext );  
	//CheckOpenNIError( eResult, "Create image generator" );  
	// 5. set map mode 
	XnMapOutputMode mapMode; 
	mapMode.nXRes = 640; 
	mapMode.nYRes = 480;  
	mapMode.nFPS =30; //帧数 
	eResult = mDepthGenerator.SetMapOutputMode( mapMode ); 
	eResult = mImageGenerator.SetMapOutputMode( mapMode );  

	// 6. correct view port 
	mDepthGenerator.GetAlternativeViewPointCap().SetViewPoint( mImageGenerator ); //匹配深度图和rgb图

	// 7. tart generate data  
	eResult = mContext.StartGeneratingAll();
	// 8. read data 
	eResult = mContext.WaitNoneUpdateAll();

	dep16Img = cvCreateImage(cvSize(640,480),IPL_DEPTH_16U,1);
	bgrImg = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
	depthImg = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);

	depthImg1 = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);

	rgbImg = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
	//clusterImg = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
	StartBtnFlag = 1;
	//RRT=0;
	//hc=cvCreateImage(cvSize(640,480),IPL_DEPTH_16U,1);
	
	// initialize the selected point
	selectedPoint.X = selectedPoint.Y = selectedPoint.Z = 0;
	//写入图像
	image = cvCreateImage(cvSize(640,480),8,1);//不要写在onTimer里
	cvNamedWindow("result",1);
	SetTimer(0,100,NULL);
	
	char txtName[30]; 
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	int second = LOBYTE(systime.wSecond);
	int minute = LOBYTE(systime.wMinute);
	int hour = LOBYTE(systime.wHour) + 8;
	hour = hour%24;
	int year = systime.wYear;
	int month = LOBYTE(systime.wMonth);
	int day = LOBYTE(systime.wDay);
	char str_second[10],str_minute[10],str_hour[10],str_month[10],str_day[10],str_year[10];
	sprintf_s(str_second,"%02d",second);
	sprintf_s(str_minute,"%02d",minute);
	sprintf_s(str_hour,"%02d",hour);
	sprintf_s(str_day,"%02d",day);
	sprintf_s(str_month,"%02d",month);
	_itoa_s(year,str_year,10,10);
	strcpy_s(txtName,30,str_year);
	strcat_s(txtName,30,str_month);
	strcat_s(txtName,30,str_day);
	strcat_s(txtName,30,"_");
	strcat_s(txtName,30,str_hour);
	strcat_s(txtName,30,str_minute);


	char dir_command[100];
	char * file_dir = "F:\\projects\\robot\\RobotControl-20150908\\img-save\\";

	strcpy_s(dir_command,100,"md ");
	strcat_s(dir_command,100,file_dir);
	strcat_s(dir_command,100,txtName);
	system(dir_command);//创建目录  

	strcpy(txtfile,file_dir);

	strcat_s(txtfile,100,txtName);//先创建目录再往目录里写文件
	strcat_s(txtfile,100,"\\");

	strcat(txtfile,txtName);
	strcat(txtfile,".txt");
	endposition = fopen(txtfile, "wt+");
	strcpy(savefile,file_dir);
	strcat(savefile,txtName);
}
