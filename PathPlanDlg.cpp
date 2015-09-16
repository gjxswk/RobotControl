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
bool flag_check;
bool saveimg;
bool hz_first=1;
bool hz_second=1;

int k = 0;
char txtfile[100];
char savefile[100];
float z_min;
int ttt;
float z_t1=0.0f;
float z_t2=0.0f;

float m_posx;
float m_posy;
float m_posz;

bool matlab_eng;
extern bool handclose;
extern bool handopen;
extern bool flag_throw;
extern bool flag_zero;
extern bool flag_move1;
extern bool flag_move2;

XnPoint3D selectedPoint;
XnPoint3D base; 

XnDepthPixel *hz1=new XnDepthPixel[640*480]; //背景                                             //???????????????????????
XnDepthPixel *hz2=new XnDepthPixel[640*480]; //实时场景信息                                     //???????????????????????
XnDepthPixel *hz3=new XnDepthPixel[640*480]; //中转 
XnDepthPixel *hz22=new XnDepthPixel[640*480]; //实时场景信息

FILE* endposition;

CvMemStorage * stor;
CvSeq *cont;

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
		ini_ang[i] = m_JNT[i]*PI/180;
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
	ON_BN_CLICKED(IDC_BUTTON_CATCHMODE, &CPathPlanDlg::OnButtonCatchMode)
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

void CPathPlanDlg::DrawPicToHDC(IplImage* img, unsigned int ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img,img->nChannels);
	cimg.DrawToHDC(hDC,&rect);
	ReleaseDC(pDC);
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
			return;
		}
		qmtomModule();
		ShowJntVariable();
		if (next_ang[1]<-PI||next_ang[1]>PI)
		{
			KillTimer(0);
			AfxMessageBox("关节1越界");
			return;
		}
		if (next_ang[2]<-PI/2||next_ang[2]>PI/2)
		{
			KillTimer(0);
			AfxMessageBox("关节2越界");
			return;
		}
		if (next_ang[3]<-PI||next_ang[3]>PI)
		{
			KillTimer(0);
			AfxMessageBox("关节3越界");
			return;
		}
		if (next_ang[4]<-2*PI/3||next_ang[4]>2*PI/3)
		{
			KillTimer(0);
			AfxMessageBox("关节4越界");
			return;
		}
		if (next_ang[5]<-PI||next_ang[5]>PI)
		{
			KillTimer(0);
			AfxMessageBox("关节5越界");
			return;
		}
		if (next_ang[6]<-5*PI/6||next_ang[6]>5*PI/6)
		{
			KillTimer(0);
			AfxMessageBox("关节6越界");
			return;
		}
		if ( next_ang[7]<-PI || next_ang[7]>PI )
		{
			KillTimer(0);
			AfxMessageBox("关节7越界");
			return;
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
	if (nIDEvent == 5)
	{
		eResult = mContext.WaitNoneUpdateAll();	///刷新数据的备份区域
		depthMD = mDepthGenerator.GetDepthMap(); 
		imgMD = mImageGenerator.GetImageMap();

		memcpy(dep16Img->imageData,depthMD,640*480*2);

		cvConvertScale(dep16Img,depthImg,255/4096.0,0);	
		//cvConvertScale(dep16Img,depthImg1,255/4096.0,0);	

		memcpy(bgrImg->imageData,imgMD,640*480*3);
		cvCvtColor(bgrImg,rgbImg,CV_RGB2BGR);
		mDepthGenerator.GetMetaData(mDepthMD);
	
		/****************** annotation by gjx ********************/
		// DrawPicToHDC(depthImg, IDC_STATIC_DEPTH);
		/*********************************************************/

		if (flag_check==0)
		{
			if (saveimg == 1)
			{
				/*k=1;*/
				char imgName_d[100];
				char imgName_c[100];
				char str_imgName[30];
				char tempstr_d[100];
				char tempstr_c[100];
				strcpy_s(imgName_d,30,"d_");
				strcpy_s(imgName_c,30,"c_");
				sprintf_s(str_imgName,"%06d",k);
				strcat_s(imgName_d,30,str_imgName);
				strcat_s(imgName_c,30,str_imgName);
				strcat_s(imgName_d,30,".bmp");
				strcat_s(imgName_c,30,".bmp");

				strcpy_s(tempstr_d,100,savefile);
				strcat_s(tempstr_d,100,"\\");
				strcpy_s(tempstr_c,100,savefile);
				strcat_s(tempstr_c,100,"\\");

				strcat(tempstr_d,imgName_d);//必须要加一个中间变量  否则覆盖
				strcat(tempstr_c,imgName_c);
				cvSaveImage(tempstr_d,depthImg);
				cvSaveImage(tempstr_c,rgbImg);
				//cvSaveImage(savefile,rgbImg);
				k++;
			}
		}

		XnPoint3D *Point3D_image=new XnPoint3D[640*480];
		//XnPoint3D *Point3D_real=new XnPoint3D[640*480];
		int idxShift,idx;
		for(int j=0;j<480;j++)
		{	
			idxShift=j*640;
			for(int i=0;i<640;i++)
			{
				idx=idxShift+i;
				Point3D_image[idx].X=i;
				Point3D_image[idx].Y=j;
				Point3D_image[idx].Z=mDepthMD[idx];
			}
		}
		
		static int index[640*480];
		int indexnum=0;
		memset(hz3,0,sizeof(XnDepthPixel));
	
		for(int i=0;i<640*480;i++)
		{
			//hz3[i]=depthMD[i];


			index[i]=0.0;

			//if(Point3D_real[i].X>5&&Point3D_real[i].X<300&&Point3D_real[i].Y<500&&Point3D_real[i].Y>-500)///////选择感兴趣的部分        index=0时会出现错误  故如果选择的感兴趣的区域没有障碍物就会在运行时出错
			//if(Point3D_image[i].X>290&&Point3D_image[i].X<370&&Point3D_image[i].Y<140&&Point3D_image[i].Y>50)
			if(Point3D_image[i].X>350&&Point3D_image[i].X<480&&Point3D_image[i].Y<220&&Point3D_image[i].Y>100)
			//if(Point3D_image[i].X>280&&Point3D_image[i].X<400&&Point3D_image[i].Y<220&&Point3D_image[i].Y>100)//
			{
				hz3[i]=depthMD[i];
			}
		}
	
		if (hz_second==1) //去除程序开始时圈定范围的黑框
		{
			memcpy(hz1,hz3,640*480*2);
		}
		if (hz_first==0)
		{
			memcpy(hz1,hz3,640*480*2);
			hz_first=1;
			hz_second=0;
		}

		if (hz_first==1)
		{
		
			memcpy(hz2,hz3,640*480*2);


			memcpy(hz22,hz3,640*480*2);
		}
		/*float z_min=0.0;*/

	
		for (int j=0;j<640*480;j++)
		{
			if(abs(hz2[j]-hz1[j])<=32)//4cm  40/(5000/4096)
			//if(hz2[j]-hz1[j]<=32)
			{
				hz2[j]=4000;
			}
			else
			{
				index[indexnum]=j;	
				indexnum++;

				hz2[j]=0;//背景相减后深度值大于4cm的显示黑色

			}
		}
		z_min=1600.0;//基座标

		for (int i=0;i<indexnum;i++)
		{
			if(depthMD[(index[i])]!=0 && z_min<1673)
			{
				if ( z_min>depthMD[(index[i])]/*z_min<(1673-depthMD[(index[i])])*/)
				{
					z_min=depthMD[(index[i])];
				}
			}
		
		}

		for (int j=0;j<640*480;j++)
		{
			if(abs(hz22[j]-hz1[j])>32 &&hz22[j]>z_min&& hz22[j]<=z_min+20)//4cm  40/(5000/4096)
				//if(hz2[j]-hz1[j]<=32)
			{
				hz22[j]=0;
			}
			else
			{
				hz22[j]=4000;
			}
		}


		cv::Mat depth_diff(480,640,CV_16UC1,hz2);
		cvConvertScale(&IplImage(depth_diff),depthImg,255/4096.0,0);

		cv::Mat depth_diff1(480,640,CV_16UC1,hz22);
		cvConvertScale(&IplImage(depth_diff1),depthImg1,255/4096.0,0);
 		//memcpy(dep16Img->imageData,hz2,640*480*2);
 		//cvConvertScale(dep16Img,depthImg,255/4096.0,0);
	
	
		//DrawPicToHDC(depthImg, IDC_STATIC_DEPTH);


		cvShowImage("result",depthImg);
		//cvReleaseImage(&depthImg);
		//cvThreshold(depthImg,dst,30,255,CV_THRESH_BINARY);

 		stor=cvCreateMemStorage(0);
		cont=cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),stor);

		int pixelx_green = 0;//区分顶面和侧面
		int pixely_green = 0;

		int pixelx_red = 0;
		int pixely_red = 0;

		float x=0.0;
		float y=0.0;
		float z=0.0;
		float z_r=0.0;
		float z_g=0.0;
	
 		cvFindContours(depthImg,stor,&cont,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
		for (;cont;cont=cont->h_next)
			{
				CvRect r=((CvContour*)cont)->rect;
				if (r.height*r.width<9000&&r.height*r.width>200)//判断语句必须加  否则输出的永远是320 240
				{
					cvRectangle(rgbImg,cvPoint(r.x,r.y),cvPoint(r.x+r.width,r.y+r.height),CV_RGB(255,0,0),1,CV_AA,0); //画红框
					pixelx_red=r.x+0.5*r.width;
					pixely_red=r.y+0.5*r.height;
					z_r=Point3D_image[pixely_red*640+pixelx_red].Z;//写在里面
				}
			
			}




 		cvFindContours(depthImg1,stor,&cont,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
 		for (;cont;cont=cont->h_next)
 		{
 			CvRect r=((CvContour*)cont)->rect;
 			if (r.height*r.width<9000&&r.height*r.width>200)//判断语句必须加  否则输出的永远是320 240
 			{
 				cvRectangle(rgbImg,cvPoint(r.x,r.y),cvPoint(r.x+r.width,r.y+r.height),CV_RGB(0,255,0),1,CV_AA,0); //画绿框
 				pixelx_green=r.x+0.5*r.width;
 				pixely_green=r.y+0.5*r.height;
 				z_g=Point3D_image[pixely_green*640+pixelx_green].Z;//写在里面
 			}
 
 		}
	    
		/**************************annotated by gjx************************************/
		//DrawPicToHDC(rgbImg, IDC_STATIC_COLOR); 
		/******************************************************************************/
	
	   
	

		float cx=292.46;
		float cy=215.492;
		float fx=485.426;
		float fy=487.471;

		float inv_T[4][4]={{-0.9976, -0.0690, 0.0012,-0.5152},
							{-0.0687, 0.9944, 0.0799, 0.1527},
							{-0.0067,0.0796 ,-0.9968, 2.0735},
							{0 , 0, 0 ,1.0000}};//标定矩阵，可以自己做标定
		float c[4]={0.0};
		float multipy[4] = {0.0};
		CString s;

		//////////////////////////鼠标响应点坐标显示

		selectedPoint.Z = mDepthMD[int(selectedPoint.Y*640+selectedPoint.X)];
		s.Format("%.2f %.2f %.2f", selectedPoint.X, selectedPoint.Y, selectedPoint.Z);

		/* annotated by gjx */
		// SetDlgItemText(IDC_IMAGECORD, s);
		/******************************************/
		//SetDlgItemText(IDC_EDIT1, s);

		if (pixelx_green==0&&pixely_green==0)
		{
			if (pixelx_red==0&&pixely_red==0)
			{
				base.X = 0.0f;//基座标
				base.Y = 0.0f;
				base.Z = 0.0f;
				s.Format("%f  %f  %f",
					0.0,0.0,0.0);

				// annotated by gjx 
				//SetDlgItemText(IDC_EDIT2,s);
				/************************************/
			}
			
			else
			{
				z=z_r;
				x=((pixelx_red-cx)/fx)*z*0.001;
				y=((pixely_red-cy)/fy)*z*0.001;
				c[0]= x;
				c[1]= y;
				c[2]= z*0.001;
				c[3]= 1;
				Array_Multipy(inv_T,c,multipy);
				s.Format("%f  %f  %f",
					base.X,base.Y-0.15,base.Z+0.05);//减15cm是因为标定时导轨处于中间位置  现在因空间不足将导轨正向移动了15cm
				/************** annotated by gjx *********************/
				//SetDlgItemText(IDC_EDIT2,s);
				/******************************************************/
			}
		} 
		else
		{
			z=z_g;
			x=((pixelx_green-cx)/fx)*z*0.001;
			y=((pixely_green-cy)/fy)*z*0.001;
			c[0]= x;
			c[1]= y;
			c[2]= z*0.001;
			c[3]= 1;
			Array_Multipy(inv_T,c,multipy);
			s.Format("%f  %f  %f",
				base.X,base.Y-0.15,base.Z+0.05);//-0.15机械臂的真实坐标
			/************** annotated by gjx ***************************/
			// SetDlgItemText(IDC_EDIT2,s);
			/***********************************************************/

		}

		//base.Z=0.62;
		float zz=0.0;
		zz=base.Z;
		
		
		if ( zz>=0.5&&zz<0.9)//区别静止和运动
		{
			
			if (flag_check==1)//机械臂初始化之后，才执行下面的动作
			{
				if (ttt==0)//
				{
					z_t1=base.Z;
					if(z_t2 == 0.0f)
						z_t2 = z_t1;
					else
					{
						if (abs(z_t2-z_t1)<0.1)
						{
							flag_check=0;
							SetTimer(5,100,NULL);//0.1秒取一次值
							if ((0.70+base.Z)/2<0.64)
							{//桌子高度0.7，取目标物中点
								(0.70+base.Z)/2 == 0.645;//机械臂的安全高度  ------??? what is he doing?
							}
							// assign the goal position and draw
							doc->drawGoalFlag = doc_real->drawGoalFlag = true;
							doc->goalPos[0] = doc_real->goalPos[0] = base.X;
							doc->goalPos[1] = doc_real->goalPos[1] = base.Y;
							doc->goalPos[2] = doc_real->goalPos[2] = base.Z;
							UpdateData(FALSE);
							view->InvalidateRect(NULL, FALSE);
							// 往matlab 写入目标点信息goal.txt写入目标的坐标信息
							FILE* goal = fopen("D:\\Cconnection5\\matlabRRT\\goal.txt", "wt");
							/*fprintf( goal, "%f  %f  %f\r\n",base.X,base.Y-0.09,base.Z+0.05);*/
							//base.X = -0.7323; base.Y = -0.1291; base.Z = 0.6666;
							//fprintf(goal, "%f  %f  %f\r\n",base.X,base.Y,base.Z);
							fprintf(goal, "%f  %f  %f\r\n", base.X+0.3, base.Y-0.08, (0.70+base.Z)/2+0.03);//相对于己坐标系
							fclose(goal);


							base.Z=0.0;


							/******************* added by gjx, don't use the MATLAB function ************
							//调用MATLAB
							matlab_eng=1;
							HANDLE hthread;
							hthread = CreateThread(NULL,0,OnBnClickedButtonGo,(LPVOID)this,0,NULL);
							CloseHandle(hthread);
							//hMutex = CreateMutex(NULL,false,NULL);
                            **********************************************************/

							

							Sleep(10);
							}
						}
						z_t2 = z_t1;
					}
				}
				ttt++;
				if (ttt==20)
				{
					ttt=0;
				}
			
		}

		
		
		
		
		//////////////////////////鼠标响应点坐标显示
		
		/*selectedPoint.Z = mDepthMD[int(selectedPoint.Y*640+selectedPoint.X)];
		s.Format("%.2f %.2f %.2f", selectedPoint.X, selectedPoint.Y, selectedPoint.Z);
		//SetDlgItemText(IDC_IMAGECORD, s);
		SetDlgItemText(IDC_EDIT1, s);*/

		



		delete[] Point3D_image;
		//delete[] Point3D_real;
			
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

	fp = fopen(filename+"预编程关节基座数据.txt","w");
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

void CPathPlanDlg::Array_Multipy(float inv_T[4][4], float c[4], float multipy[4])
{//图像坐标系向基座标系转换
	int i,t;
	float temp = 0;
// 	int a1 = M, a2 = N, b2 = K;			//数组维数

	for(i = 0; i < 4; i++)
	{
		
			temp = 0;   
			for(t = 0; t < 4; t++)
			{   
				temp += inv_T[i][t] * c[t];   
			}   
			multipy[i] = temp;   
	  
	} 
	base.X=multipy[0];
	base.Y=multipy[1];
	base.Z=multipy[2];
    
}

void CPathPlanDlg::OnButtonCamera()
{
	// TODO: 在此添加控件通知处理程序代码

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

	// 7. start generate data  
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
	SetTimer(5,100,NULL);
	
	// get the system time, set up the write/save directory
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
	char * file_dir = "D:\\projects\\robot\\RobotControl-20150908\\img-save\\";

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

/*
**get the current angel of each joint, to set the variable 'ini_ang'
*/
void CPathPlanDlg::getCurrentJointAngel() {
	CMainFrame *pframe;
	pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
	int device=pframe->pSiderBar->pCtrlTab->pInitHardware->m_device;

	ini_ang[0] = 0.0f;
	float pos = 0.0;
	for (int i = 1; i < 8; i++) {
		::PCube_getPos(device, i, &pos);
		ini_ang[i] = pos;
	}
}

/*
**this function used to respond to the "catch" button. It get the current goal position from
**the goal file which is written by camera method. Then it use the linear move function to 
**move the arm.
*/
void CPathPlanDlg::OnButtonCatchMode() {
	FILE* goal = fopen("D:\\Cconnection5\\matlabRRT\\goal.txt", "r");
							/*fprintf( goal, "%f  %f  %f\r\n",base.X,base.Y-0.09,base.Z+0.05);*/
							//base.X = -0.7323; base.Y = -0.1291; base.Z = 0.6666;
							//fprintf(goal, "%f  %f  %f\r\n",base.X,base.Y,base.Z);
	double ee[6];
    fscanf(goal, "%f  %f  %f\r\n", &ee[0], &ee[1], &ee[2]);//相对于己坐标系
	fclose(goal);

	Forwardkine_static(ini_ang, PEint);
	int i = 0; 
	for (; i < 6; i++)
		PEend[i] = PEint[i];
	PEend[0] = ee[0];
	PEend[1] = ee[1];
	PEend[2] = ee[2];

	// PEend: 3 to 5, how to define them is to be test.

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



/*********************added by gjx, not use the MATLAB  **********************
DWORD WINAPI OnBnClickedButtonGo(LPVOID lpParameter)
{//调用matlab程序
	
	CPathPlanDlg* tempCrobot = (CPathPlanDlg *)lpParameter;

// 	FILE* goal = fopen("D:\\RRT\\7Dof  9.25\\goal.txt", "wt");
// 	/*fprintf( goal, "%f  %f  %f\r\n",base.X,base.Y-0.09,base.Z+0.05);*/
// 	fprintf( goal, "%f  %f  %f\r\n",base.X,base.Y-0.09,(0.65+base.Z)/2);//相对于己坐标系
// 	fclose(goal);
// 	if (base.Z>=0.55)
// 	{



// 		FILE* goal = fopen("D:\\RRT\\7Dof  9.25\\goal.txt", "wt");
// 		/*fprintf( goal, "%f  %f  %f\r\n",base.X,base.Y-0.09,base.Z+0.05);*/
// 		fprintf( goal, "%f  %f  %f\r\n",base.X,base.Y-0.09,(0.65+base.Z)/2);//相对于己坐标系
// 		fclose(goal);



		// 	char txtName[30]; 
		// 	SYSTEMTIME systime;
		// 	GetSystemTime(&systime);
		// 	int second = LOBYTE(systime.wSecond);
		// 	int minute = LOBYTE(systime.wMinute);
		// 	int hour = LOBYTE(systime.wHour) + 8;
		// 	hour = hour%24;
		// 	int year = systime.wYear;
		// 	int month = LOBYTE(systime.wMonth);
		// 	int day = LOBYTE(systime.wDay);
		// 	char str_second[10],str_minute[10],str_hour[10],str_month[10],str_day[10],str_year[10];
		// 	sprintf_s(str_second,"%02d",second);
		// 	sprintf_s(str_minute,"%02d",minute);
		// 	sprintf_s(str_hour,"%02d",hour);
		// 	sprintf_s(str_day,"%02d",day);
		// 	sprintf_s(str_month,"%02d",month);
		// 	_itoa_s(year,str_year,10,10);
		// 	strcpy_s(txtName,30,str_year);
		// 	strcat_s(txtName,30,str_month);
		// 	strcat_s(txtName,30,str_day);
		// 	strcat_s(txtName,30,"_");
		// 	strcat_s(txtName,30,str_hour);
		// 	strcat_s(txtName,30,str_minute);
		// 	
		// 	
		// 	char dir_command[100];
		// 	char * file_dir = "C:\\Users\\robot\\Desktop\\robot-71\\save\\";
		// 
		// 	strcpy_s(dir_command,100,"md ");
		// 	strcat_s(dir_command,100,file_dir);
		// 	strcat_s(dir_command,100,txtName);
		// 	system(dir_command);//创建目录  
		// 
		// 	strcpy(txtfile,file_dir);
		// 
		// 	strcat_s(txtfile,100,txtName);//先创建目录再往目录里写文件
		// 	strcat_s(txtfile,100,"\\");
		// 
		// 	strcat(txtfile,txtName);
		// 	strcat(txtfile,".txt");
		// 	strcpy(savefile,file_dir);
		// 	strcat(savefile,txtName);


		// 	FILE* endposition = fopen(txtfile, "at");//不覆盖原数据
		// 	fprintf( endposition, "%f %f %f\r\n", m_posx,m_posy,m_posz);
		// 	fclose(endposition);


		// TODO: 在此添加控件通知处理程序代码


//	ArmCommand ac;
//
//	if (ArmCommand::GenCommand(ac, "Zero"))
//	{
//		tempCrobot->arm.RunCommand(ac);
//	}
//	else
//	{
//		AfxMessageBox(_T("Bad Parameters"));
//	}
//
//	
//
//	//--Matlab---
//	if (matlab_eng==1)//matlab 运行完了,跟VC接上。
//	{
//		Engine *ep /*= NULL*/;
//		if(!(ep=engOpen(NULL)))
//			::MessageBox(NULL,"Can' start the MATLAB engine","VC调用matlab engine示例程序",MB_OK);//不能注释掉
//		//engEvalString(ep,"addpath('D:\\RRT\\7Dof  9.25');");
//		engEvalString(ep,"addpath('D:\\Cconection5\\matlabRRT');");
//		engEvalString(ep,"main_0521;");//执行matlab的String 
//		//::MessageBox(NULL,"按任意键继续","VC调用matlab engine示例程序 by h&h",MB_OK);
//		engClose(ep);   
//		matlab_eng=0;
//	}
//	// matlab_eng=0;   
//	if (matlab_eng==0)
//	{
//		ArmCommand ac;
//
//		//SetTimer(1,100,NULL);
//		//Sleep(10);
//		if (ArmCommand::GenCommand(ac, "Line"))
//		{
//			tempCrobot->arm.RunCommand(ac);
//		}
//		else
//		{
//			AfxMessageBox(_T("2Bad Parameters"));
//		}
//	}
//	//-----------
//
//	if (handclose==1)
//	{
//		HandCommand hc;
//
//		if (HandCommand::GenCommand(hc, "Close", 1, 1, 1, 0))
//		{
//			tempCrobot->hand.RunCommand(hc);
//		}
//		else
//		{
//			AfxMessageBox(_T("Bad Parameters"));
//		}	
//	}
//   
//	//flag_throw=1;
//
//	if (flag_throw==1)
//	{
//		ArmCommand ac;
//
//		if (ArmCommand::GenCommand(ac, "Throw"))
//		{
//			tempCrobot->arm.RunCommand(ac);
//		}
//		else
//		{
//			AfxMessageBox(_T("1Bad Parameters"));
//		}
//	}
//
//	if (handopen==1)
//	{
//		HandCommand hc;
//
//		if (HandCommand::GenCommand(hc, "Open", 1, 1, 1, 0))
//		{
//			tempCrobot->hand.RunCommand(hc);
//		}
//		else
//		{
//			AfxMessageBox(_T("Bad Parameters"));
//		}	
//	}
//
//	
//		//flag_check=1;
//		//Sleep(1000);
//	if (flag_zero==1)
//	{
//		/*OnBnClickedButtonUninitialize();*/
//		ArmCommand ac;
//
//		if (ArmCommand::GenCommand(ac, "Zero"))
//		{
//			tempCrobot->arm.RunCommand(ac);
//		}
//		else
//		{
//
//			AfxMessageBox(_T("Bad Parameters"));
//		}
//	}
//	handclose=0;//不能在外面赋值  否则每次进行操作都需要重新执行程序
//	handopen=0;
//	flag_throw=0;
//	flag_zero=0;
//	flag_check=0;
//	ttt=0;
//
//	return 0;
///*}*/
//	
//}