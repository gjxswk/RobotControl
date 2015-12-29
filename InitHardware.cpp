// InitHardware.cpp : implementation file
//

#include "stdafx.h"

#include "RobotControl.h"
#include "InitHardware.h"
#include "m5apiw32.h"
#include "MainFrm.h"

#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CODEANGTRANS 22.7555555555555556
#define PI 3.14159265359
#define DIAMETER 0.058
float velocity;
char stralpha[50];
char outstr1[100]="F";

#define RAMPMODE 0;
#define VELOCITYMODE 1;
#define CURRUNTMODE 2;
/////////////////////////////////////////////////////////////////////////////
// CInitHardware dialog

CMainFrame *pframe;
CInitHardware::CInitHardware(CWnd* pParent /*=NULL*/)
	: CDialog(CInitHardware::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInitHardware)
	m_mail = _T("");
	m_velocity = 0.0f;
	m_absoposition = 0.0f;
	m_relaposition = 0.0f;
	m_PosModule1 = 0.0f;
	m_PosModule2 = 0.0f;
	m_PosModule3 = 0.0f;
	m_VelModule1 = 0.0f;
	m_VelModule2 = 0.0f;
	m_VelModule3 = 0.0f;
	m_VelModule4 = 0.0f;
	m_VelModule5 = 0.0f;
	m_VelModule6 = 0.0f;
	m_PosModule4 = 0.0f;
	m_PosModule5 = 0.0f;
	m_PosModule6 = 0.0f;
	m_AccModule1 = 0.0f;
	m_AccModule2 = 0.0f;
	m_AccModule3 = 0.0f;
	m_AccModule4 = 0.0f;
	m_AccModule5 = 0.0f;
	m_AccModule6 = 0.0f;
	m_VelModule7 = 0.0f;
	m_PosModule7 = 0.0f;
	m_AccModule7 = 0.0f;
	m_ActPos = _T("");
	m_ActVel = _T("");
	ModuleSelect=8;
	m_DeltaPos = _T("");
	PmacstateValue=0;
	PMacdevice=0;
	//}}AFX_DATA_INIT
}


void CInitHardware::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitHardware)
	DDX_Control(pDX, IDC_STATIC_DELTAPOS, m_CDeltaPos);
	DDX_Control(pDX, IDC_STATIC_ACTVEL, m_CActVel);
	DDX_Control(pDX, IDC_STATIC_ACTPOS, m_CActPos);
	DDX_Control(pDX, IDC_LIST_MODULE, m_ListCtrl);
	DDX_Text(pDX, IDC_STATIC_PMACMAIL, m_mail);
	DDX_Text(pDX, IDC_EDIT_VELOCITY, m_velocity);
	DDV_MinMaxFloat(pDX, m_velocity, 0.f, 500.f);
	DDX_Text(pDX, IDC_EDIT_ABSOPOSITION, m_absoposition);
	DDV_MinMaxFloat(pDX, m_absoposition, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_EDIT_RELAPOSITION, m_relaposition);
	DDV_MinMaxFloat(pDX, m_relaposition, -1000.f, 1000.f);
	DDX_Text(pDX, IDC_EDIT_POSMODULE1, m_PosModule1);
	DDX_Text(pDX, IDC_EDIT_POSMODULE2, m_PosModule2);
	DDX_Text(pDX, IDC_EDIT_POSMODULE3, m_PosModule3);
	DDX_Text(pDX, IDC_EDIT_VELMODULE1, m_VelModule1);
	DDX_Text(pDX, IDC_EDIT_VELMODULE2, m_VelModule2);
	DDX_Text(pDX, IDC_EDIT_VELMODULE3, m_VelModule3);
	DDX_Text(pDX, IDC_EDIT_VELMODULE4, m_VelModule4);
	DDX_Text(pDX, IDC_EDIT_VELMODULE5, m_VelModule5);
	DDX_Text(pDX, IDC_EDIT_VELMODULE6, m_VelModule6);
	DDX_Text(pDX, IDC_EDIT_POSMODULE4, m_PosModule4);
	DDX_Text(pDX, IDC_EDIT_POSMODULE5, m_PosModule5);
	DDX_Text(pDX, IDC_EDIT_POSMODULE6, m_PosModule6);
	DDX_Text(pDX, IDC_EDIT_ACCMODULE1, m_AccModule1);
	DDX_Text(pDX, IDC_EDIT_ACCMODULE2, m_AccModule2);
	DDX_Text(pDX, IDC_EDIT_ACCMODULE3, m_AccModule3);
	DDX_Text(pDX, IDC_EDIT_ACCMODULE4, m_AccModule4);
	DDX_Text(pDX, IDC_EDIT_ACCMODULE5, m_AccModule5);
	DDX_Text(pDX, IDC_EDIT_ACCMODULE6, m_AccModule6);
	DDX_Text(pDX, IDC_EDIT_VELMODULE7, m_VelModule7);
	DDX_Text(pDX, IDC_EDIT_POSMODULE7, m_PosModule7);
	DDX_Text(pDX, IDC_EDIT_ACCMODULE7, m_AccModule7);
	DDX_Text(pDX, IDC_STATIC_ACTPOS, m_ActPos);
	DDX_Text(pDX, IDC_STATIC_ACTVEL, m_ActVel);
	DDX_Text(pDX, IDC_STATIC_DELTAPOS, m_DeltaPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInitHardware, CDialog)
	//{{AFX_MSG_MAP(CInitHardware)
	ON_BN_CLICKED(IDC_BUTTON_PMACOPEN, OnButtonPmacopen)
	ON_BN_CLICKED(IDC_BUTTON_PMACCLOSE, OnButtonPmacclose)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, OnButtonZero)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_MAKESURE, OnButtonMakesure)
	ON_BN_CLICKED(IDC_BUTTON_RUN1, OnButtonRun1)
	ON_BN_CLICKED(IDC_BUTTON_RUN2, OnButtonRun2)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_INITMODULE, OnInitmodule)
	ON_COMMAND(IDM_RESETMODULE, OnResetmodule)
	ON_COMMAND(IDM_HALTMODULE, OnHaltmodule)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MODULE, OnClickListModule)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_GO1, OnButtonGo1)
	ON_BN_CLICKED(IDC_BUTTON_GO2, OnButtonGo2)
	ON_BN_CLICKED(IDC_BUTTON_GO3, OnButtonGo3)
	ON_BN_CLICKED(IDC_BUTTON_GO4, OnButtonGo4)
	ON_BN_CLICKED(IDC_BUTTON_GO5, OnButtonGo5)
	ON_BN_CLICKED(IDC_BUTTON_GO6, OnButtonGo6)
	ON_BN_CLICKED(IDC_BUTTON_GO7, OnButtonGo7)
	ON_BN_CLICKED(IDC_RADIO_MODULE1, OnRadioModule1)
	ON_BN_CLICKED(IDC_RADIO_MODULE2, OnRadioModule2)
	ON_BN_CLICKED(IDC_RADIO_MODULE3, OnRadioModule3)
	ON_BN_CLICKED(IDC_RADIO_MODULE4, OnRadioModule4)
	ON_BN_CLICKED(IDC_RADIO_MODULE5, OnRadioModule5)
	ON_BN_CLICKED(IDC_RADIO_MODULE6, OnRadioModule6)
	ON_BN_CLICKED(IDC_RADIO_MODULE7, OnRadioModule7)
	//}}AFX_MSG_MAP
//	ON_STN_CLICKED(IDC_STATIC_ACTPOS, &CInitHardware::OnStnClickedStaticActpos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInitHardware message handlers
///////////////////////////////////////////////////////////////////////////
///////////////////////////导轨控制部分////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void CInitHardware::OnButtonPmacopen() 
{
	// TODO: Add your control notification handler code here
	if (OpenPmacDevice( PMacdevice))
	{
		PmacstateValue = 1;
		m_mail="Open Pmac Device Successfully!";  
		
	}
	else
	{
		PmacstateValue = 0;
		m_mail="Open Pmac Device failed!";
	}
	UpdateData(false);
	
}

void CInitHardware::OnButtonPmacclose() 
{
	// TODO: Add your control notification handler code here
	if (1 == PmacstateValue)
	{
		PmacGetResponseA(PMacdevice,buf,255,"j/");
		PmacGetResponseA(PMacdevice,buf,255,"k");
	}
	PmacstateValue = !ClosePmacDevice(1);
	m_mail="Pmac Device is Closed!";
	UpdateData(false);	
	
}

void CInitHardware::OnButtonZero() 
{
	// TODO: Add your control notification handler code here
	TCHAR buffer[255];
	float v=0,p;
	PmacGetResponseA(PMacdevice,buffer,255,"p");
	p=atof(buffer);
	if (p>0)
	{
		PmacGetResponseA(PMacdevice,buffer,255,"i123=-32");
		PmacGetResponseA(PMacdevice,buffer,255,"#1HM");
		
		while(v!=0.0|fabs(p)>30.0)
		{
			
			PmacGetResponseA(PMacdevice,buffer,255,"v");
			v=atof(buffer);
			PmacGetResponseA(PMacdevice,buffer,255,"p");
			p=atof(buffer);
		}
		PmacGetResponseA(PMacdevice,buffer,255,"k");
		m_mail="Home Complete!";
		UpdateData(false);
		
		
	}
	else
	{
		PmacGetResponseA(PMacdevice,buffer,255,"i123=32");
		PmacGetResponseA(PMacdevice,buffer,255,"#1HM");
		
		while(v!=0.0|abs(p)>30.0)
		{
			
			PmacGetResponseA(PMacdevice,buffer,255,"v");
			v=atof(buffer);
			PmacGetResponseA(PMacdevice,buffer,255,"p");
			p=atof(buffer);
		}
		PmacGetResponseA(PMacdevice,buffer,255,"k");
		m_mail="Home Complete!";
		UpdateData(false);
		
		
	}
	
}

void CInitHardware::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	TCHAR buffer[255];
	float p;
	PmacGetResponseA(PMacdevice,buffer,255,"p");
	p=atof(buffer);
	while (abs(p)>50)
	{
		m_mail="Home Firstly!";
		UpdateData(false);
		OnButtonZero();
		PmacGetResponseA(PMacdevice,buffer,255,"p");
		p=atof(buffer);
		
	}
	
	PmacGetResponseA(PMacdevice,buffer,255,"$$$***");
	PmacDownloadA(PMacdevice,NULL,NULL,NULL,"E:\822.pmc",1,1,1,1);
	PmacGetResponseA(PMacdevice,buffer,255,"sav");
	m_mail="Reset Complete!";
	UpdateData(false);
	
}

void CInitHardware::OnButtonMakesure() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	velocity=(360*30*CODEANGTRANS*m_velocity)/(PI*DIAMETER*1000);
	_gcvt(velocity,45,stralpha);
	strcat(outstr1,stralpha);
	m_mail="Velocity Set By Ready!";
	UpdateData(false);
	
}

void CInitHardware::OnButtonRun1() 
{
	// TODO: Add your control notification handler code here
	TCHAR buffer[255];
	char strbita[50];
	char outstr2[100]="X";
	float velocity,position;
	UpdateData(true);

	position=(360*30*CODEANGTRANS*m_absoposition)/(PI*DIAMETER*1000);
	_gcvt(position,45,strbita);
	strcat(outstr2,strbita);

	PmacGetResponseA(PMacdevice,buffer,255,"j/");
	PmacGetResponseA(PMacdevice,buffer,255,"&1");
	PmacGetResponseA(PMacdevice,buffer,255,"CLOSE");
	PmacGetResponseA(PMacdevice,buffer,255,"#1->X");
	PmacGetResponseA(PMacdevice,buffer,255,"OPEN PROG1");
	PmacGetResponseA(PMacdevice,buffer,255,"CLEAR");
	PmacGetResponseA(PMacdevice,buffer,255,"LINEAR");
	PmacGetResponseA(PMacdevice,buffer,255,"ABS");
	PmacGetResponseA(PMacdevice,buffer,255,"TA50");
	PmacGetResponseA(PMacdevice,buffer,255,"TS0");
	PmacGetResponseA(PMacdevice,buffer,255,outstr1);
	PmacGetResponseA(PMacdevice,buffer,255,outstr2);
	PmacGetResponseA(PMacdevice,buffer,255,"CLOSE");
	
	
	PmacGetResponseA(PMacdevice,buffer,255,"&1 B1 R");
	float p,v;
	PmacGetResponseA(PMacdevice,buffer,255,"p");
	p=atof(buffer);
	PmacGetResponseA(PMacdevice,buffer,255,"v");
	v=atof(buffer);
	while(abs(p-position)>50||v!=0)
	{
		PmacGetResponseA(PMacdevice,buffer,255,"p");
		p=atof(buffer);
		PmacGetResponseA(PMacdevice,buffer,255,"v");
		v=atof(buffer);

		PmacGetResponseA(PMacdevice,buffer,255,"?"); 
		char b=buffer[9];
		if(b=='C')
		{
			b=12;
		}
		if(b&8)
		{
			
			PmacGetResponseA(PMacdevice,buffer,255,"k");
			m_mail="Stop On Position Limit!";
			UpdateData(false);
			break;
		}
	}
		PmacGetResponseA(PMacdevice,buffer,255,"k");
				if(m_mail!="Stop On Position Limit!")
				{
            	   m_mail="Motion End!";
	               UpdateData(false);
				}

	
	
}

void CInitHardware::OnButtonRun2() 
{
	// TODO: Add your control notification handler code here
	TCHAR buffer[255];
	char strbita[50];
	char outstr2[100]="X";
	float position,p1;
	PmacGetResponseA(PMacdevice,buffer,255,"p");
	p1=atof(buffer);
	UpdateData(true);
	
	position=(360*30*CODEANGTRANS*m_relaposition)/(PI*DIAMETER*1000);
	_gcvt(position,45,strbita);
	strcat(outstr2,strbita);
	
	PmacGetResponseA(PMacdevice,buffer,255,"j/");
	PmacGetResponseA(PMacdevice,buffer,255,"&1");
	PmacGetResponseA(PMacdevice,buffer,255,"CLOSE");
	PmacGetResponseA(PMacdevice,buffer,255,"#1->X");
	PmacGetResponseA(PMacdevice,buffer,255,"OPEN PROG1");
	PmacGetResponseA(PMacdevice,buffer,255,"CLEAR");
	PmacGetResponseA(PMacdevice,buffer,255,"LINEAR");
	PmacGetResponseA(PMacdevice,buffer,255,"INC");
	PmacGetResponseA(PMacdevice,buffer,255,"TA50");
	PmacGetResponseA(PMacdevice,buffer,255,"TS0");
	PmacGetResponseA(PMacdevice,buffer,255,outstr1);
	PmacGetResponseA(PMacdevice,buffer,255,outstr2);
	PmacGetResponseA(PMacdevice,buffer,255,"CLOSE");
	
	
	PmacGetResponseA(PMacdevice,buffer,255,"&1 B1 R");
	
	float p,v;
	PmacGetResponseA(PMacdevice,buffer,255,"p");
	p=atof(buffer);
	PmacGetResponseA(PMacdevice,buffer,255,"v");
	v=atof(buffer);
	while(abs(p-(position+p1))>50||v!=0)
	{
		PmacGetResponseA(PMacdevice,buffer,255,"p");
		p=atof(buffer);
		PmacGetResponseA(PMacdevice,buffer,255,"v");
		v=atof(buffer);

		PmacGetResponseA(PMacdevice,buffer,255,"?"); 
		char b=buffer[9];
		if(b=='C')
		{
			b=12;
		}
		if(b&8)
		{
			
			PmacGetResponseA(PMacdevice,buffer,255,"k");
			m_mail="Stop On Position Limit!";
			UpdateData(false);
			break;
		}
	}
		PmacGetResponseA(PMacdevice,buffer,255,"k");
				if(m_mail!="Stop On Position Limit!")
				{
            	   m_mail="Motion End!";
	               UpdateData(false);
				}
	
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////导轨控制部分结束////////////////////////////////
///////////////////////////////////////////////////////////////////////////
BOOL CInitHardware::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_ListCtrl.InsertColumn(0, "ID /  Type  / Version / S.N.", LVCFMT_CENTER, 150, -1 );//插入列，位置0 
	m_ListCtrl.InsertColumn(1, "Status", LVCFMT_CENTER, 300, 0 );//插入列，位置1 

	m_ActPos="act pos: 0.0000 degree";
	m_ActVel="act vel: 0.0000 rad/s";
	m_DeltaPos="delta pos: 0.0000 degree";
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInitHardware::OnButtonScan() 
{
	// TODO: Add your control notification handler code here
	
	int ThreadID;
	pframe=(CMainFrame*)::AfxGetApp()->GetMainWnd();
	m_hThread = CreateThread( NULL,
								0,
								(LPTHREAD_START_ROUTINE)Scanning,
								(LPVOID)this,
								0,
								(LPDWORD)&ThreadID);
	
	if ( !m_hThread )  //如果创建线程失败
	{
		AfxMessageBox("Thread not running");
	}
	else   //创建线程成功
	{
		m_mail.Format( "Scanning for PowerCube Modules ...",0 );
		GetDlgItem(IDC_STATIC_PMACMAIL)->SetWindowText("Scanning for PowerCube Modules ...");
	}
}

int CInitHardware::Scanning()
{
	int retval;
	int deviceadr;
	int i;
	CString m_strInit;
	char buffer[255];
//	char* pInitString="VCI:1,D000,5";
	if(::GetPrivateProfileString( "HARDWARE", "INITSTRING", "", buffer, 255, "C:\\Windows\\PCubeDemo.ini" ) )
		m_strInit.Format(buffer);
	retval=::PCube_openDevice((int *)&deviceadr,(char *) (m_strInit.GetBuffer(m_strInit.GetLength())));//打开设备
	if (retval==0)
	{
		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_STATIC_PMACMAIL)->SetWindowText("Open Device Successfully!");

	}
	else
	{
		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_STATIC_PMACMAIL)->SetWindowText("Open Device failed!");	
	}
	int CubeID[31];
	pframe->pSiderBar->pCtrlTab->pInitHardware->m_device=deviceadr;
	retval=::PCube_getModulesIdMap(deviceadr,CubeID);  //扫描模块
	if (retval == 0)  //检测到0个模块
	{
//		errorBeep();
		AfxMessageBox("CLDERR_DRIVE_NOMODULEFOUND");
	}
	else if (retval == CLERR_WRONGHANDLE)
	{
//		errorBeep();
		AfxMessageBox("CLERR_WRONGHANDLE");
	}
	else				//正常情况
	{		
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount = retval;//将检测到的模块数目副给m_intCount
		retval = CLD_OK;
		for (int i=0;i<pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount;i++)
		{
			pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].m_bModuleSelected=false;
		}
	}
	for (i=0;i<pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount;i++)
	{
		pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].m_CubeID=CubeID[i];
	}
	for (i=0;i<pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount;i++)
	{
		::PCube_getModuleType(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].ModuleType);
		::PCube_getDefCubeVersion(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefCubeVersion);
		::PCube_getDefSerialNo(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefSerialNo);
		::PCube_getDefHomeOffset(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefHomeOffset);
		::PCube_getDefMinPos(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefMinPos);
		::PCube_getDefMaxPos(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefMaxPos);
		::PCube_getDefMaxDeltaPos(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefMaxDeltaPos);
		::PCube_getDefMaxVel(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefMaxVel);
		::PCube_getDefMaxCur(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefMaxCur);
		::PCube_getDefMaxAcc(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefMaxACC);
		::PCube_getDefHomeVel(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefHomeVel);
		::PCube_getDefConfig(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefConfig);
	
	}

/////////////////////////在控件中显示模块//////////////////////////////////////////
	CString strText;
	if ( pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount)
	{		
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_ListCtrl.DeleteAllItems();  //清空列表框内容

		for (i=0;i<pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount;i++ )
		{
			strText.Format( TEXT( "%.3d Rotary Drive %.5x #%.5d" ), 
			/*pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].m_logicCubeID*/CubeID[i], 
			pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefCubeVersion,
			pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DefSerialNo);
			pframe->pSiderBar->pCtrlTab->pInitHardware->m_ListCtrl.InsertItem( i, strText );
			pframe->pSiderBar->pCtrlTab->pInitHardware->FillStatusCol( i );
		}
		CString txt;
		txt.Format(" Modules found: %d",pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount); 
		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_STATIC_PMACMAIL)->SetWindowText(txt);
///////////////////////////点亮运动操作部分//////////////////////////////////////////////////////////////////
//		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_RADIO_POSITION)->EnableWindow(true);
//		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_RADIO_VELOCITY)->EnableWindow(true);
//		pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_RADIO_CURRENT)->EnableWindow(true);

		for (i=0;i<pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount;i++)
		{
			switch(CubeID[i])
			{
			case 1:
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_AccModule1=4.0;
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_VelModule1=2.0;
				break;
			case 2:
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_AccModule2=4.0;
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_VelModule2=2.0;
				break;
			case 3:
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_AccModule3=4.0;
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_VelModule3=2.0;
				break;
			case 4:
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_AccModule4=4.0;
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_VelModule4=2.0;
				break;
			case 5:
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_AccModule5=4.0;
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_VelModule5=2.0;
				break;
			case 6:
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_AccModule6=4.0;
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_VelModule6=2.0;
				break;
			case 7:
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(true);
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_AccModule7=4.0;
				pframe->pSiderBar->pCtrlTab->pInitHardware->m_VelModule7=2.0;
				break;
			default:
				break;
			}

		}
	}
	else
	{
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_ListCtrl.DeleteAllItems();
	}
	
	while(1)
	{
		for (i=0;i<pframe->pSiderBar->pCtrlTab->pInitHardware->m_intCount;i++)
		{
			::PCube_getCubeState(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].ModuleState);
			::PCube_getActPos(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].ActPos);
			::PCube_getActVel(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].ActVel);
			::PCube_getDeltaPos(deviceadr,CubeID[i],&pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].DeltaPos);
			pframe->pSiderBar->pCtrlTab->pInitHardware->FillStatusCol( i );
			
		}
		
		if(pframe->pSiderBar->pCtrlTab->pInitHardware->ModuleSelect!=8)
		{
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_ActPos.Format("act pos: %.5f degree",pframe->pSiderBar->pCtrlTab->pInitHardware->GetDegreeFromRad(pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[pframe->pSiderBar->pCtrlTab->pInitHardware->ModuleSelect].ActPos));
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_ActVel.Format("act vel: %.5f rad/s",pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[pframe->pSiderBar->pCtrlTab->pInitHardware->ModuleSelect].ActVel);
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_DeltaPos.Format("delta pos: %.5f degree",pframe->pSiderBar->pCtrlTab->pInitHardware->GetDegreeFromRad(pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[pframe->pSiderBar->pCtrlTab->pInitHardware->ModuleSelect].DeltaPos));
		
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_CActPos.SetWindowText(pframe->pSiderBar->pCtrlTab->pInitHardware->m_ActPos);
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_CActVel.SetWindowText(pframe->pSiderBar->pCtrlTab->pInitHardware->m_ActVel);
		pframe->pSiderBar->pCtrlTab->pInitHardware->m_CDeltaPos.SetWindowText(pframe->pSiderBar->pCtrlTab->pInitHardware->m_DeltaPos);
		}
		//	UpdateData(false);

	}
		
}


void CInitHardware::FillStatusCol( int i )
{
	///////////////////////////////////////////////////////////////////
	// Filling with Status
	///////////////////////////////////////////////////////////////////

	CString strText;
	CString strOK;
	//strOK.Format( ( "EEE " ) );
	strOK.Format(  ("------- ")  );
	unsigned long tempstate;

	tempstate = pframe->pSiderBar->pCtrlTab->pInitHardware->Parameter[i].ModuleState;

	if ( tempstate & STATE_HOME_OK )
		strText.Format(  "HOK "  );
	else
		strText.Format( (strOK ) );
	if ( tempstate & STATE_BRAKEACTIVE )
		strText+= "BRA ";
	else
		strText+= strOK;
	if ( tempstate & STATE_CURLIMIT )
		strText+= "LIM ";
	else
		strText+= strOK;
	if ( tempstate & STATE_HALTED )
		strText+= "HLT ";
	else
		strText+= strOK;
	if ( tempstate & STATE_MOTION )
		strText+= "MOV ";
	else
		strText+= strOK;
	if ( tempstate & STATE_ERROR )
		strText+= "ERR " ;
	else
		strText+= strOK ;
///////////////////////////////////////
	if ( tempstate & STATE_SWR )
		strText+= "SWR ";
	else
		strText+= strOK;
	if ( tempstate & STATE_SW1 )
		strText+= "SW1 ";
	else
		strText+= strOK;
	if ( tempstate & STATE_SW2 )
		strText+= "SW2 ";
	else
		strText+= strOK;
//////////////////////////////////////////////
	if ( tempstate & STATE_RAMP_ACC )
		strText+= "ACC ";
	else
		strText+= strOK;
	if ( tempstate & STATE_RAMP_DEC )
		strText+= "DEC ";
	else
		strText+= strOK;
	if ( tempstate & STATE_INPROGRESS )
		strText+= "PRG ";
	else
		strText+= strOK;
	if ( tempstate & STATE_RAMP_STEADY )
		strText+= "STD ";
	else
		strText+= strOK;
	if ( tempstate & STATE_RAMP_END )
		strText+= "END ";
	else
		strText+= strOK;
	if ( tempstate & STATE_FULLBUFFER )
		strText+= "FUL ";
	else
		strText+= strOK;
/////////////////////////////////////////////
	if ( tempstate & STATE_TOW_ERROR )
		strText+= "TOW ";
	else
		strText+= strOK;
	if ( tempstate & STATE_COMM_ERROR )
		strText+= "COM ";
	else
		strText+= strOK;
	if ( tempstate & STATE_CPU_OVERLOAD )
		strText+= "CPU ";
	else
		strText+= strOK;
	if ( tempstate & STATE_POWERFAULT )
		strText+= "POW ";
	else
		strText+= strOK;
	if ( tempstate & STATE_BEYOND_HARD )
		strText+= "HRD ";
	else
		strText+= strOK;
	if ( tempstate & STATE_BEYOND_SOFT )
		strText+="SOF ";
	else
		strText+= strOK;
	if ( tempstate & STATE_POW_VOLT_ERR )
		strText+= "VLT ";
	else
		strText+= strOK;
	if ( tempstate & STATE_POW_FET_TEMP )
		strText+= "FET ";
	else
		strText+= strOK;
	if ( tempstate & STATE_POW_WDG_TEMP )
		strText+= "WDG ";
	else
		strText+= strOK;
	if ( tempstate & STATE_POW_SHORTCUR )
		strText+= "SHT ";
	else
		strText+= strOK;
	if ( tempstate & STATE_POW_HALLERR )
		strText+= "HAL ";
	else
		strText+= strOK;
	if ( tempstate & STATE_POW_INTEGRALERR )
		strText+= "INT ";
	else
		strText+= strOK;
	
pframe->pSiderBar->pCtrlTab->pInitHardware->m_ListCtrl.SetItemText( i, 1, strText );	
//	m_strFillStatCol.Format( strText );
	
}

void CInitHardware::OnContextMenu(CWnd* pWnd, CPoint point) ///////右键弹出菜单
{
	// TODO: Add your message handler code here
	

	CMenu menu;
	menu.LoadMenu(IDR_MENUCONTEXT);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	
}

void CInitHardware::OnInitmodule() 
{
	// TODO: Add your command handler code here
	int retval;
	int i;
	CString txt;
	for(i=0;i<m_intCount;i++)
	{
		if(Parameter[i].m_bModuleSelected)
		{
			::PCube_haltModule(m_device,Parameter[i].m_CubeID);
			retval=::PCube_syncModule(m_device,Parameter[i].m_CubeID);
			if (retval!=0)
			{
				m_mail="Init failed!";
				UpdateData(false);
			}
			else
			{
				txt.Format("Module %.3d initialized!",Parameter[i].m_CubeID);
				m_mail=txt;
				UpdateData(false);
				for(i=0;i<m_intCount;i++)
				{
					Parameter[i].m_bModuleSelected=false;
				}
			}

		}


	}


}

void CInitHardware::OnResetmodule() 
{
	// TODO: Add your command handler code here
	int retval;
	int i;
	CString txt;
	for(i=0;i<m_intCount;i++)
	{
		if(Parameter[i].m_bModuleSelected)
		{
			::PCube_resetModule(m_device,Parameter[i].m_CubeID);
			txt.Format("Module %.3d Reseted!",Parameter[i].m_CubeID);
			m_mail=txt;
			UpdateData(false);
			for(i=0;i<m_intCount;i++)
				{
					Parameter[i].m_bModuleSelected=false;
				}
		}
		
	}
}

void CInitHardware::OnHaltmodule() 
{
	// TODO: Add your command handler code here
	int retval;
	int i;
	CString txt;
	for(i=0;i<m_intCount;i++)
	{
		if(Parameter[i].m_bModuleSelected)
		{
			::PCube_haltModule(m_device,Parameter[i].m_CubeID);
			txt.Format("Module %.3d Halted!",Parameter[i].m_CubeID);
			m_mail=txt;
			UpdateData(false);
			for(i=0;i<m_intCount;i++)
				{
					Parameter[i].m_bModuleSelected=false;
				}
		}
		
	}
	
}

void CInitHardware::OnClickListModule(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int i,nItem;
	POSITION pos=m_ListCtrl.GetFirstSelectedItemPosition();
	if (pos==NULL)
		TRACE0("No items were selected!\n");
	else
	{

			nItem=m_ListCtrl.GetNextSelectedItem(pos);
			TRACE1("Item %d was Selected!\n",nItem);
			Parameter[nItem].m_bModuleSelected=true;

	}
	*pResult = 0;
}

void CInitHardware::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	DWORD ExCode = STILL_ACTIVE;
	int i;
//	int deviceadr;
	for (i=0;i<m_intCount;i++)
	{
		::PCube_haltModule(m_device,Parameter[i].m_CubeID);
	}

	m_mail="Terminating Thread, Closing Demo...please wait";
	UpdateData(false);
	TerminateThread(m_hThread,0);		
	//Wait until Thread is finished
	while ( 1 )
	{
		GetExitCodeThread( m_hThread,&ExCode );
		if ( ExCode != STILL_ACTIVE )
			break;
	}
	::PCube_closeDevice(m_device);
	m_ListCtrl.DeleteAllItems();
	for (i=0;i<m_intCount;i++)
	{
		Parameter[i].m_bModuleSelected=false;

	}
	m_mail="";
	UpdateData(false);
}


float CInitHardware::GetRadFromDegree(float deg)
{
	return ( deg * 2.0f * PI / 360.0f );
}
float CInitHardware::GetDegreeFromRad(float rad)
{
	return ( rad * 360.0f / (2.0f * PI) );
}
void CInitHardware::OnButtonGo1() 
{
	// TODO: Add your control notification handler code here
	ModuleSelect=0;
	CString txt;
	UpdateData(true);
	::PCube_resetModule(m_device,1);
	if(GetRadFromDegree(m_PosModule1)>Parameter[0].DefMaxPos && GetRadFromDegree(m_PosModule1)<Parameter[0].DefMinPos)
	{
		txt.Format("Range should be in [%.3d degree, %.3d degree]",GetDegreeFromRad(Parameter[0].DefMinPos),GetDegreeFromRad(Parameter[0].DefMaxPos));
		AfxMessageBox(txt);
	}
	if (GetRadFromDegree(m_VelModule1)>Parameter[0].DefMaxVel && GetRadFromDegree(m_VelModule1)<-Parameter[0].DefMaxVel)
	{
		txt.Format("Range should be in [-%.3d degree/s, %.3d degree/s]",GetDegreeFromRad(Parameter[0].DefMaxVel),GetDegreeFromRad(Parameter[0].DefMaxVel));
		AfxMessageBox(txt);
	}
	if (GetRadFromDegree(m_AccModule1)>Parameter[0].DefMaxACC && GetRadFromDegree(m_AccModule1)<-Parameter[0].DefMaxACC)
	{
		txt.Format("Range should be in [-%.3d degree/s^2, %.3d degree/s^2]",GetDegreeFromRad(Parameter[0].DefMaxACC),GetDegreeFromRad(Parameter[0].DefMaxACC));
		AfxMessageBox(txt);
	}       
	::PCube_moveRamp(m_device,
					1,
					GetRadFromDegree(m_PosModule1),
					GetRadFromDegree(m_VelModule1),
					GetRadFromDegree(m_AccModule1) );
	
//	m_ActPos.Format("act pos: %.5f degree",GetDegreeFromRad(Parameter[0].ActPos));
//	m_ActVel.Format("act vel: %.5f rad/s",Parameter[0].ActVel);
//	UpdateData(false);
	GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE1))->SetCheck(false);

	
}

void CInitHardware::OnButtonGo2() 
{
	// TODO: Add your control notification handler code here
	ModuleSelect=1;
	CString txt;
	UpdateData(true);
	::PCube_resetModule(m_device,2);
		if(GetRadFromDegree(m_PosModule2)>Parameter[1].DefMaxPos && GetRadFromDegree(m_PosModule2)<Parameter[1].DefMinPos)
		{
			txt.Format("Range should be in [%.3d degree, %.3d degree]",GetDegreeFromRad(Parameter[1].DefMinPos),GetDegreeFromRad(Parameter[1].DefMaxPos));
			AfxMessageBox(txt);
		
		}
		if (GetRadFromDegree(m_VelModule2)>Parameter[1].DefMaxVel && GetRadFromDegree(m_VelModule2)<-Parameter[1].DefMaxVel)
		{
			txt.Format("Range should be in [-%.3d degree/s, %.3d degree/s]",GetDegreeFromRad(Parameter[1].DefMaxVel),GetDegreeFromRad(Parameter[1].DefMaxVel));
			AfxMessageBox(txt);
		
		}
		if (GetRadFromDegree(m_AccModule2)>Parameter[1].DefMaxACC && GetRadFromDegree(m_AccModule2)<-Parameter[1].DefMaxACC)
		{
			txt.Format("Range should be in [-%.3d degree/s^2, %.3d degree/^2]",GetDegreeFromRad(Parameter[1].DefMaxACC),GetDegreeFromRad(Parameter[1].DefMaxACC));
			AfxMessageBox(txt);
		
		}       
		::PCube_moveRamp(m_device,
			2,
			GetRadFromDegree(m_PosModule2),
			GetRadFromDegree(m_VelModule2),
			GetRadFromDegree(m_AccModule2) );

//	m_ActPos.Format("act pos: %.5f degree",GetDegreeFromRad(Parameter[1].ActPos));
//	m_ActVel.Format("act vel: %.5f rad/s",Parameter[1].ActVel);
//	UpdateData(false);

	GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE2))->SetCheck(false);

}

void CInitHardware::OnButtonGo3() 
{
	// TODO: Add your control notification handler code here
	ModuleSelect=2;
	CString txt;
	UpdateData(true);
	::PCube_resetModule(m_device,3);
		if(GetRadFromDegree(m_PosModule3)>Parameter[2].DefMaxPos && GetRadFromDegree(m_PosModule3)<Parameter[2].DefMinPos)
		{
			txt.Format("Range should be in [%.3d degree, %.3d degree]",GetDegreeFromRad(Parameter[2].DefMinPos),GetDegreeFromRad(Parameter[2].DefMaxPos));
			AfxMessageBox(txt);
			
		}
		if (GetRadFromDegree(m_VelModule3)>Parameter[2].DefMaxVel && GetRadFromDegree(m_VelModule3)<-Parameter[2].DefMaxVel)
		{
			txt.Format("Range should be in [-%.3d degree/s, %.3d degree/s]",GetDegreeFromRad(Parameter[2].DefMaxVel),GetDegreeFromRad(Parameter[2].DefMaxVel));
			AfxMessageBox(txt);
		
		}
		if (GetRadFromDegree(m_AccModule3)>Parameter[2].DefMaxACC && GetRadFromDegree(m_AccModule3)<-Parameter[2].DefMaxACC)
		{
			txt.Format("Range should be in [-%.3d degree/s^2, %.3d degree/s^2]",GetDegreeFromRad(Parameter[2].DefMaxACC),GetDegreeFromRad(Parameter[2].DefMaxACC));
			AfxMessageBox(txt);
		
		}       
		::PCube_moveRamp(m_device,
			3,
			GetRadFromDegree(m_PosModule3),
			GetRadFromDegree(m_VelModule3),
			GetRadFromDegree(m_AccModule3) );
//	m_ActPos.Format("act pos: %.5f degree",GetDegreeFromRad(Parameter[2].ActPos));
//	m_ActVel.Format("act vel: %.5f rad/s",Parameter[2].ActVel);
//	UpdateData(false);

	GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE3))->SetCheck(false);

}

void CInitHardware::OnButtonGo4() 
{
	// TODO: Add your control notification handler code here
	ModuleSelect=3;
	CString txt;
	UpdateData(true);
	::PCube_resetModule(m_device,4);
		if(GetRadFromDegree(m_PosModule4)>Parameter[3].DefMaxPos && GetRadFromDegree(m_PosModule4)<Parameter[3].DefMinPos)
		{
			txt.Format("Range should be in [%.3d degree, %.3d degree]",GetDegreeFromRad(Parameter[3].DefMinPos),GetDegreeFromRad(Parameter[3].DefMaxPos));
			AfxMessageBox(txt);
		
		}
		if (GetRadFromDegree(m_VelModule4)>Parameter[3].DefMaxVel && GetRadFromDegree(m_VelModule4)<-Parameter[3].DefMaxVel)
		{
			txt.Format("Range should be in [-%.3d degree/s, %.3d degree/s]",GetDegreeFromRad(Parameter[3].DefMaxVel),GetDegreeFromRad(Parameter[3].DefMaxVel));
			AfxMessageBox(txt);
			
		}
		if (GetRadFromDegree(m_AccModule4)>Parameter[3].DefMaxACC && GetRadFromDegree(m_AccModule4)<-Parameter[3].DefMaxACC)
		{
			txt.Format("Range should be in [-%.3d degree/s^2, %.3d degree/s^2]",GetDegreeFromRad(Parameter[3].DefMaxACC),GetDegreeFromRad(Parameter[3].DefMaxACC));
			AfxMessageBox(txt);
		
		}       
		::PCube_moveRamp(m_device,
			4,
			GetRadFromDegree(m_PosModule4),
			GetRadFromDegree(m_VelModule4),
			GetRadFromDegree(m_AccModule4) );

//	m_ActPos.Format("act pos: %.5f degree",GetDegreeFromRad(Parameter[3].ActPos));
//	m_ActVel.Format("act vel: %.5f rad/s",Parameter[3].ActVel);
//	UpdateData(false);

	GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE4))->SetCheck(false);

	
}

void CInitHardware::OnButtonGo5() 
{
	// TODO: Add your control notification handler code here
	ModuleSelect=4;
	CString txt;
	UpdateData(true);
	::PCube_resetModule(m_device,5);
		if(GetRadFromDegree(m_PosModule5)>Parameter[4].DefMaxPos && GetRadFromDegree(m_PosModule5)<Parameter[4].DefMinPos)
		{
			txt.Format("Range should be in [%.3d degree, %.3d degree]",GetDegreeFromRad(Parameter[4].DefMinPos),GetDegreeFromRad(Parameter[4].DefMaxPos));
			AfxMessageBox(txt);
		
		}
		if (GetRadFromDegree(m_VelModule5)>Parameter[4].DefMaxVel && GetRadFromDegree(m_VelModule5)<-Parameter[4].DefMaxVel)
		{
			txt.Format("Range should be in [-%.3d degree/s, %.3d degree/s]",GetDegreeFromRad(Parameter[4].DefMaxVel),GetDegreeFromRad(Parameter[4].DefMaxVel));
			AfxMessageBox(txt);
		
		}
		if (GetRadFromDegree(m_AccModule5)>Parameter[4].DefMaxACC && GetRadFromDegree(m_AccModule5)<-Parameter[4].DefMaxACC)
		{
			txt.Format("Range should be in [-%.3d degree/s^2, %.3d degree/s^2]",GetDegreeFromRad(Parameter[4].DefMaxACC),GetDegreeFromRad(Parameter[4].DefMaxACC));
			AfxMessageBox(txt);
		
		}       
		::PCube_moveRamp(m_device,
			5,
			GetRadFromDegree(m_PosModule5),
			GetRadFromDegree(m_VelModule5),
			GetRadFromDegree(m_AccModule5) );

//	m_ActPos.Format("act pos: %.5f degree",GetDegreeFromRad(Parameter[4].ActPos));
//	m_ActVel.Format("act vel: %.5f rad/s",Parameter[4].ActVel);
//	UpdateData(false);

	GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE5))->SetCheck(false);

	
}

void CInitHardware::OnButtonGo6() 
{
	// TODO: Add your control notification handler code here
	ModuleSelect=5;
	CString txt;
	UpdateData(true);
	::PCube_resetModule(m_device,6);
		if(GetRadFromDegree(m_PosModule6)>Parameter[5].DefMaxPos && GetRadFromDegree(m_PosModule6)<Parameter[5].DefMinPos)
		{
			txt.Format("Range should be in [%.3d degree, %.3d degree]",GetDegreeFromRad(Parameter[5].DefMinPos),GetDegreeFromRad(Parameter[5].DefMaxPos));
			AfxMessageBox(txt);
		
		}
		if (GetRadFromDegree(m_VelModule6)>Parameter[5].DefMaxVel && GetRadFromDegree(m_VelModule6)<-Parameter[5].DefMaxVel)
		{
			txt.Format("Range should be in [-%.3d degree/s, %.3d degree/s]",GetDegreeFromRad(Parameter[5].DefMaxVel),GetDegreeFromRad(Parameter[5].DefMaxVel));
			AfxMessageBox(txt);
			
		}
		if (GetRadFromDegree(m_AccModule6)>Parameter[5].DefMaxACC && GetRadFromDegree(m_AccModule6)<-Parameter[5].DefMaxACC)
		{
			txt.Format("Range should be in [-%.3d degree/s^2, %.3d degree/s^2]",GetDegreeFromRad(Parameter[5].DefMaxACC),GetDegreeFromRad(Parameter[5].DefMaxACC));
			AfxMessageBox(txt);
		
		}       
		::PCube_moveRamp(m_device,
			6,
			GetRadFromDegree(m_PosModule6),
			GetRadFromDegree(m_VelModule6),
			GetRadFromDegree(m_AccModule6) );

//	m_ActPos.Format("act pos: %.5f degree",GetDegreeFromRad(Parameter[5].ActPos));
//	m_ActVel.Format("act vel: %.5f rad/s",Parameter[5].ActVel);
//	UpdateData(false);

	GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE6))->SetCheck(false);

	
}

void CInitHardware::OnButtonGo7() 
{
	// TODO: Add your control notification handler code here
	ModuleSelect=6;
	CString txt;
	UpdateData(true);
	::PCube_resetModule(m_device,7);
	if(GetRadFromDegree(m_PosModule7)>Parameter[6].DefMaxPos && GetRadFromDegree(m_PosModule7)<Parameter[6].DefMinPos)
	{
		txt.Format("Range should be in [%.3d degree, %.3d degree]",GetDegreeFromRad(Parameter[6].DefMinPos),GetDegreeFromRad(Parameter[6].DefMaxPos));
		AfxMessageBox(txt);
		
	}
	if (GetRadFromDegree(m_VelModule7)>Parameter[6].DefMaxVel && GetRadFromDegree(m_VelModule7)<-Parameter[6].DefMaxVel)
	{
		txt.Format("Range should be in [-%.3d degree/s, %.3d degree/s]",GetDegreeFromRad(Parameter[6].DefMaxVel),GetDegreeFromRad(Parameter[6].DefMaxVel));
		AfxMessageBox(txt);
		
	}
	if (GetRadFromDegree(m_AccModule7)>Parameter[6].DefMaxACC && GetRadFromDegree(m_AccModule7)<-Parameter[6].DefMaxACC)
	{
		txt.Format("Range should be in [-%.3d degree/s^2, %.3d degree/s^2]",GetDegreeFromRad(Parameter[6].DefMaxACC),GetDegreeFromRad(Parameter[6].DefMaxACC));
		AfxMessageBox(txt);
		
	}       
	::PCube_moveRamp(m_device,
		7,
		GetRadFromDegree(m_PosModule7),
		GetRadFromDegree(m_VelModule7),
		GetRadFromDegree(m_AccModule7) );
	
//	m_ActPos.Format("act pos: %.5f degree",GetDegreeFromRad(Parameter[6].ActPos));
//	m_ActVel.Format("act vel: %.5f rad/s",Parameter[6].ActVel);
//	UpdateData(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE7))->SetCheck(false);

	
}

void CInitHardware::OnRadioModule1() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_MODULE1))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE2))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE3))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE4))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE5))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE6))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE7))->SetCheck(false);

	GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(true);

	GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(false);

	GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(false);

	GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(false);

	GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(false);

	GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(false);
}

void CInitHardware::OnRadioModule2() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_MODULE1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE2))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE3))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE4))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE5))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE6))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE7))->SetCheck(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(true);
	
	GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(false);

	
}

void CInitHardware::OnRadioModule3() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_MODULE1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE2))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE3))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE4))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE5))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE6))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE7))->SetCheck(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(true);
	
	GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(false);
	
}

void CInitHardware::OnRadioModule4() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_MODULE1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE2))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE3))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE4))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE5))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE6))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE7))->SetCheck(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(true);
	
	GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(false);
	
}

void CInitHardware::OnRadioModule5() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_MODULE1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE2))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE3))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE4))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE5))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE6))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE7))->SetCheck(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(true);
	
	GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(false);
	
}

void CInitHardware::OnRadioModule6() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_MODULE1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE2))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE3))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE4))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE5))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE6))->SetCheck(true);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE7))->SetCheck(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(true);
	
	GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(false);
	
}

void CInitHardware::OnRadioModule7() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_RADIO_MODULE1))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE2))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE3))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE4))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE5))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE6))->SetCheck(false);
	((CButton*)GetDlgItem(IDC_RADIO_MODULE7))->SetCheck(true);
	
	GetDlgItem(IDC_EDIT_POSMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO1)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO2)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO3)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO4)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE5)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO5)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_VELMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ACCMODULE6)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GO6)->EnableWindow(false);
	
	GetDlgItem(IDC_EDIT_POSMODULE7)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_VELMODULE7)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ACCMODULE7)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_GO7)->EnableWindow(true);
	
}


void CInitHardware::OnStnClickedStaticActpos()
{
	// TODO: 在此添加控件通知处理程序代码
}
