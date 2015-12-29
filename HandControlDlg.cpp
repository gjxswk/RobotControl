// HandControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RobotControl.h"
#include "HandControlDlg.h"
#include "MainFrm.h"
#include "RobotControlDoc.h"
#include "RobotControlView.h"
#include "afxsock.h"
#include "Windows.h"
#include "m5apiw32.h"
#include "math.h"    //--乐--头文件无法打开拷贝了math.h头文件放到了工程文件夹下


#ifdef _DEBUG      //---乐---宏定义的一种
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CODEANGTRANS 22.7555555555555556
#define PI 3.14159265359f
#define DIAMETER 0.058
DWORD WINAPI HandControl(LPVOID dlg); //---乐---添加了个#define#define （？？？？）

/////////////////////////////////////////////////////////////////////////////
// CHandControlDlg dialog



CHandControlDlg::CHandControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHandControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHandControlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	kn=0;
//	No=0;
//	numtime=0;
    Mission_Number=0;
	VelValue=0.02;
	ScrValue=2;
//	CorFlag=0;
//	HandleFlag=0;
    MotionFlag=32;
	PMacdevice=0;
}


void CHandControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHandControlDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHandControlDlg, CDialog)     //---乐--宏定义一种建立消息函数一个入口
	//{{AFX_MSG_MAP(CHandControlDlg)
	ON_BN_CLICKED(IDC_RADIO_TRANSLATE, OnRadioTranslate)
	ON_BN_CLICKED(IDC_RADIO_ROTATE, OnRadioRotate)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_XTRANSP, OnButtonXtransp)
	ON_BN_CLICKED(IDC_BUTTON_YTRANSP, OnButtonYtransp)
	ON_BN_CLICKED(IDC_BUTTON_XTRANSI, OnButtonXtransi)
	ON_BN_CLICKED(IDC_BUTTON_YTRANSI, OnButtonYtransi)
	ON_BN_CLICKED(IDC_BUTTON_ZTRANSP, OnButtonZtransp)
	ON_BN_CLICKED(IDC_BUTTON_ZTRANSI, OnButtonZtransi)
	ON_BN_CLICKED(IDC_BUTTON_XROTP, OnButtonXrotp)
	ON_BN_CLICKED(IDC_BUTTON_XROTI, OnButtonXroti)
	ON_BN_CLICKED(IDC_BUTTON_YROTP, OnButtonYrotp)
	ON_BN_CLICKED(IDC_BUTTON_YROTI, OnButtonYroti)
	ON_BN_CLICKED(IDC_BUTTON_ZROTP, OnButtonZrotp)
	ON_BN_CLICKED(IDC_BUTTON_ZROTI, OnButtonZroti)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_REMOTECONTROL, OnCheckRemotecontrol)
	ON_BN_CLICKED(IDC_BUTTON_STOPH, OnButtonStoph)
	ON_BN_CLICKED(IDC_BUTTON_RESETH, OnButtonReseth)
	ON_BN_CLICKED(IDC_BUTTON_SETH, OnButtonSeth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()                             //---乐---结束宏定义

/////////////////////////////////////////////////////////////////////////////
// CHandControlDlg message handlers

void CHandControlDlg::OnRadioTranslate() 
{
	// TODO: Add your control notification handler code here
	HandleFlag=0;
	((CButton*)GetDlgItem(IDC_BUTTON_XTRANSP))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_XTRANSI))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_YTRANSP))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_YTRANSI))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_ZTRANSP))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_ZTRANSI))->EnableWindow(true);

	((CButton*)GetDlgItem(IDC_BUTTON_XROTP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_XROTI))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_YROTP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_YROTI))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_ZROTP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_ZROTI))->EnableWindow(false);
	
}

void CHandControlDlg::OnRadioRotate() 
{
	// TODO: Add your control notification handler code here
	HandleFlag=2;
	((CButton*)GetDlgItem(IDC_BUTTON_XTRANSP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_XTRANSI))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_YTRANSP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_YTRANSI))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_ZTRANSP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_ZTRANSI))->EnableWindow(false);
	
	((CButton*)GetDlgItem(IDC_BUTTON_XROTP))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_XROTI))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_YROTP))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_YROTI))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_ZROTP))->EnableWindow(true);
	((CButton*)GetDlgItem(IDC_BUTTON_ZROTI))->EnableWindow(true);
	
}

void CHandControlDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	ScrValue=((CSliderCtrl*)GetDlgItem(IDC_SLIDER_VELVALUE))->GetPos();
	UpdateData(false);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CHandControlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_BUTTON_XTRANSP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_XTRANSI))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_YTRANSP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_YTRANSI))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_ZTRANSP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_ZTRANSI))->EnableWindow(false);
	
	((CButton*)GetDlgItem(IDC_BUTTON_XROTP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_XROTI))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_YROTP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_YROTI))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_ZROTP))->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_BUTTON_ZROTI))->EnableWindow(false);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_VELVALUE))->SetRange(0,10,true);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_VELVALUE))->SetTicFreq(1);
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_VELVALUE))->SetPos(1);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHandControlDlg::OnButtonXtransp() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=128;
	SetTimer(1, 50, NULL);    //--乐--运行时间
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonYtransp() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=32;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonXtransi() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=64;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonYtransi() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=16;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonZtransp() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=8;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonZtransi() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=4;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonXrotp() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=128;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonXroti() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=64;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonYrotp() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=32;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonYroti() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=16;
	SetTimer(1,50, NULL);
    No=0;
	numtime=0;
	
}

void CHandControlDlg::OnButtonZrotp() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=8;
	SetTimer(1,50, NULL);
    No=0;
	numtime=0;
}

void CHandControlDlg::OnButtonZroti() 
{
	// TODO: Add your control notification handler code here
	MotionFlag=4;
	SetTimer(1, 50, NULL);
    No=0;
	numtime=0;
}

void CHandControlDlg::OnTimer(UINT nIDEvent) 
{
	int i;
	int SingularFlag=0;
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)		//gjx: this module means use the leg machine
		{
		ColStatusWord=MotionFlag | CorFlag | HandleFlag;
		if (HandleFlag==0)
		{
			VelValue=ScrValue*0.0002;
		}
		else
		{
			VelValue=ScrValue*PI/1800;
		}
		for(int i=0; i<8; i++)
		{
			now_ang[i]=former_ang[i];
			now_angvel[i]=former_angvel[i];
		}
		for(i=0; i<6; i++)
		{
			now_baspe[i]=former_baspe[i];
		}
		HandleOperMotionPlan(now_ang, now_baspe, ColStatusWord, VelValue, No, next_baspe,next_basvel,next_ang,next_angvel);//固定基座宇航员手动规划
		if (SingularFlag==1)
		{
			KillTimer(1);                        //--乐--销毁一个定时器
			AfxMessageBox("运动学奇异");
		}
		if (next_ang[1]<-PI||next_ang[1]>PI)  //--乐--有一个大于π或小于﹣π就提示越界，起保护作用
		{
			KillTimer(1);
			AfxMessageBox("关节1越界");
		
		}
		if (next_ang[2]<-PI/2||next_ang[2]>PI/2)  //--乐--有一个大于π/2或小于﹣π/2就提示越界，起保护作用
		{
			KillTimer(1);
			AfxMessageBox("关节2越界");
		}
		if (next_ang[3]<-PI||next_ang[3]>PI)   //--乐--有一个大于π或小于﹣π就提示越界，起保护作用
		{
			KillTimer(1);
			AfxMessageBox("关节3越界");
		}
		if (next_ang[4]<-2*PI/3||next_ang[4]>2*PI/3) //--乐--有一个大于2π/3或小于﹣2π/3就提示越界，起保护作用
		{
			KillTimer(1);
			AfxMessageBox("关节4越界");
		}
		if (next_ang[5]<-PI||next_ang[5]>PI)  //--乐--有一个大于π或小于﹣π就提示越界，起保护作用
		{
			KillTimer(1);
			AfxMessageBox("关节5越界");
		}
		if (next_ang[6]<-5*PI/6||next_ang[6]>5*PI/6)  //--乐--有一个大于5π/6或小于﹣5π/6就提示越界，起保护作用
		{
			KillTimer(1);
			AfxMessageBox("关节6越界");
		}
		if (next_ang[7]<-PI||next_ang[7]>PI)  //--乐--有一个大于π或小于﹣π就提示越界，起保护作用
		{
			KillTimer(1);
			AfxMessageBox("关节7越界");
		}
		for (i=0;i<8;i++)
		{
			former_ang[i]=next_ang[i];
			former_angvel[i]=next_angvel[i];
		}
		for(i=0; i<6; i++)
		{
			former_baspe[i]=next_baspe[i];
		}
		qmtomModule();
		ShowJntVariable();
		if (ControlMode==1)
		{
			SendMessageToJXB();			// gjx: send data to the machine
		}
		No++;
		numtime++;
	}
	view->InvalidateRect(NULL, FALSE);
	CDialog::OnTimer(nIDEvent);	// gjx: recycle for the timer
}

void CHandControlDlg::OnCheckRemotecontrol() 
{
	// TODO: Add your control notification handler code here
//	LPVOID dlg;
//	dlg = (CMainFrame*)AfxGetApp()->GetMainWnd();
	
	HANDLE pThread;
	pThread= CreateThread( NULL,0,HandControl,NULL,0,NULL);
	CloseHandle(pThread);
}

DWORD WINAPI HandControl(LPVOID dlg)
{
	AfxSocketInit();
	CString IP;
	char data[5];
	IP="59.64.138.61";
	
//	CRobotControlView* view;
//	CRobotControlDoc* doc;
	
//	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
//	CMainFrame* frame=(CMainFrame*)dlg;
//	view=(CRobotControlView*)frame->GetActiveView();
//	doc=view->GetDocument(); 
	UINT port;
	port=9300;
	CSocket m_socket,cliSock;
	m_socket.Create();
	//连接到发送端
	if(!m_socket.Connect(IP, port))
	{
		AfxMessageBox(_T("无法连接到服务器!"));
		//关闭套接字
		m_socket.Close();
		return -1;
	}
	m_socket.Listen();
	m_socket.Accept(cliSock);
	cliSock.Receive(data,5,0);


	unsigned char ColStatusWord=data[0];
	float* VelValue;
	memcpy(VelValue,data+1,4);
	
		

}
void CHandControlDlg::qmtomModule() //--乐--模型显示
{
	doc->m_Module[0].JntVar_trans=next_ang[0];
	doc->m_Module[1].JntVar_rot=next_ang[1]*180/PI;
	doc->m_Module[2].JntVar_rot=next_ang[2]*180/PI;
	doc->m_Module[3].JntVar_rot=next_ang[3]*180/PI;
	doc->m_Module[4].JntVar_rot=next_ang[4]*180/PI;
	doc->m_Module[5].JntVar_rot=next_ang[5]*180/PI;
	doc->m_Module[6].JntVar_rot=next_ang[6]*180/PI;
	doc->m_Module[7].JntVar_rot=next_ang[7]*180/PI;
	doc->No=No;
	doc->numtime=numtime;
	doc->jnt[0]=next_ang[0];
	doc->jnt[1]=next_ang[1]*180/PI;
	doc->jnt[2]=next_ang[2]*180/PI;
	doc->jnt[3]=next_ang[3]*180/PI;
    doc->jnt[4]=next_ang[4]*180/PI;
	doc->jnt[5]=next_ang[5]*180/PI;
	doc->jnt[6]=next_ang[6]*180/PI;
	doc->jnt[7]=next_ang[7]*180/PI;
	UpdateData(false); 
}

void CHandControlDlg::ShowJntVariable()
{
	int i;	
	double PEI[6], petemp[3];
	Forwardkine_static(now_ang, PEI ); 
	//	doc->Pn[No][0]=PEI[0]; 
	//	doc->Pn[No][1]=PEI[1]; 
	//	doc->Pn[No][2]=PEI[2];
    
	//	petemp[0]=PEI[0]; 
	//	petemp[1]=PEI[1]; 
	//	petemp[2]=PEI[2];
	
	////显示数据传递
	for(i=0; i<3; i++)  
	{
		doc->EndPE[i]=EndPE[No][i]=PEI[i];   //末端位姿
		doc->EndPE[i+3]=EndPE[No][i+3]=PEI[i+3]*180/PI; 
		//		doc->EndVW[i]=Ve[No][i];   //末端速度
		//		doc->EndVW[i+3]=We[No][i]*180/PI;
	}
	
	for(i=0; i<8; i++)
	{
		doc->JntW[i]= next_angvel[i]*180/PI;  //关节角速度
		//		doc->JntTau[i]= FbTau[No][i+6];
	}
	
	UpdateData(false);
}

void CHandControlDlg::OnButtonStoph() 
{
	// TODO: Add your control notification handler code here

	KillTimer(1);
	CMainFrame *pframe;
	pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
	int device=pframe->pSiderBar->pCtrlTab->pInitHardware->m_device;

	// debug message by gjx
	if (ControlMode==1)
	{	
		PmacSetIVariableDouble(0,122,0);
		for (int i=1;i<8;i++)
		{	
			PCube_setRampVel(device,i,0);
		}
	}
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
}

void CHandControlDlg::OnButtonReseth() 
{
	// TODO: Add your control notification handler code here
	No=0;
	numtime=0;
	KillTimer(1);
	int i;
	
	
	///////////////////
   	CMainFrame* frame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)frame->GetActiveView();
	doc=view->GetDocument()->doc_delay; 
	doc->m_Module[0].JntVar_trans=doc->angelset[0];
	former_ang[0]=doc->angelset[0];
	for (i=1;i<8;i++)
	{
		doc->m_Module[i].JntVar_rot=doc->angelset[i];
		former_ang[i]=doc->angelset[i]*PI/180;
	}
	
	view->InvalidateRect(NULL, FALSE);
	
}

void CHandControlDlg::OnButtonSeth() 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pframe;
	pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
	view=(CRobotControlView*)pframe->GetActiveView(); //得到当前活动的视窗指针
	doc=view->GetDocument()->doc_delay; 
	former_ang[0]=doc->jnt[0];
	former_ang[1]=doc->jnt[1]*PI/180;
	former_ang[2]=doc->jnt[2]*PI/180;
	former_ang[3]=doc->jnt[3]*PI/180;
	former_ang[4]=doc->jnt[4]*PI/180;
	former_ang[5]=doc->jnt[5]*PI/180;
	former_ang[6]=doc->jnt[6]*PI/180;
	former_ang[7]=doc->jnt[7]*PI/180;
	
	int i;
	for (i=0;i<6;i++)
	{
		former_baspe[i]=0;
	}
	for (i=0;i<8;i++)
	{
		former_angvel[i]=0;
	}
	HandleFlag=0;
	CorFlag=0;
	No=0;
	numtime=0;
	ControlMode=pframe->pSiderBar->pCtrlTab->pPathPlanDlg->ControlMode;
	((CButton*)GetDlgItem(IDC_RADIO_TRANSLATE))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_ROTATE))->EnableWindow(TRUE);
}

void CHandControlDlg::SendMessageToJXB()
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
//		::PCube_moveRamp(device,
//			i,
//			send_ang[i],
//			4*PI/180,
//			16*PI/180);
		::PCube_moveStep(device,
				i,
				send_ang[i],
				4000);

	}
	/////////////////////导轨控制//////////////////////////////
	//	VelCts=m_velocity*61440/270;
	//	PosCts=m_position*61440/270;
	VelCts=20;
	PosCts=(360*30*CODEANGTRANS*send_ang[0])/(PI*DIAMETER);
	PmacSetIVariableDouble(0,122,VelCts);
	char PosStr[100];
	char outstr[256]="#1j=";  //绝对运动指令，先电机回零
	_gcvt(PosCts,50,PosStr);
	strcat(outstr,PosStr);
	//	AfxMessageBox(outstr);
	PmacGetResponseA(PMacdevice,buf,255,outstr);
	
}
