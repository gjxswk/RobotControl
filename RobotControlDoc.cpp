// RobotControlDoc.cpp : implementation of the CRobotControlDoc class
//

#include "stdafx.h"
#include "RobotControl.h"

#include "RobotControlDoc.h"
#include "conio.h"
#include "GL/GLUT.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  BACKGROUND     1
#define  OGL_AXIS_DLIST 2
#define COMPILE false
/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc

IMPLEMENT_DYNCREATE(CRobotControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CRobotControlDoc, CDocument)
	//{{AFX_MSG_MAP(CRobotControlDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

typedef struct _FACESTRUCT
{
	float	v1[3],v2[3],v3[3];				
	float	Normal[3];					
} FaceStruct;
float a[3];
float kc=0.001;

FILE *fp;
char buf1[50], filename[20], str[50];
FaceStruct prldizuo[20000],prl120[20000],prl100[20000],prl80[20000],prl60[20000],ver120_120[10000],ver120_100[10000],ver100_100[10000],ver100_80[10000],ver80_80[10000],ver80_60[10000];
int prldizuonum,prl120num,prl100num,prl80num,prl60num,ver120_120num,ver120_100num,ver100_100num,ver100_80num,ver80_80num,ver80_60num;
/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc construction/destruction

CRobotControlDoc::CRobotControlDoc()   //---乐--模型读取函数
{
	// TODO: add one-time construction code here
	JOINT_NUM = 8;
	jnt = new float[JOINT_NUM];
	angelset = new float[JOINT_NUM];
	ini_px=ini_py=ini_pz=des_px=des_py=des_pz=0;
	ini_aif=ini_bit=ini_gam=des_aif=des_bit=des_gam=0;
	m_xRotation = 0;                         //---乐---X旋转最大极限
    m_zRotation = 0;                         //---乐---Z旋转最大极限
	m_zoom=10;                               //---乐---缩放系数
	obj=gluNewQuadric();
	jnt[0]=0.375;
	jnt[1]=-20;
	jnt[2]=25;
	jnt[3]=-45;
	jnt[4]=71;
	jnt[5]=-20;
	jnt[6]=20;
	jnt[7]=0;
	for(int i=0; i<8; i++)
	{JntW[i]=0;
	EndVW[i]=0;
	EndPE[i]=0;}

	drawGoalFlag = false;
	for (int i = 0; i < 6; i++) {
		goalPos[i] = 0.0f;
	}
	goalPos[0] = -1.0;

	m_Module[0].ModuleIndex=0;  //标号
	m_Module[0].trans.x=0; 
	m_Module[0].trans.y=0;
	m_Module[0].trans.z=0;
	m_Module[0].rot.x=0;
	m_Module[0].rot.y=0;
	m_Module[0].rot.z=0;
    m_Module[0].JntVar_rot=0.0;//关节变量
	m_Module[0].JntVar_trans=0.0;
	//base系变化到1系
	m_Module[1].ModuleIndex=1;  //标号
	m_Module[1].trans.x=0; 
	m_Module[1].trans.y=0;
	m_Module[1].trans.z=0.19;
	m_Module[1].rot.x=0;
	m_Module[1].rot.y=0;
	m_Module[1].rot.z=-7.5;
    m_Module[1].JntVar_trans=0.0;  //关节变量
    m_Module[1].JntVar_trans1=0.6;  //关节变量
	m_Module[1].JntVar_rot=0.0; 
	
	//1系变化到2系
	m_Module[2].ModuleIndex=2;
	m_Module[2].trans.x=-0.09;
	m_Module[2].trans.y=0;
	m_Module[2].trans.z=0.11;//0.385+0.015
	m_Module[2].rot.x=0;
	m_Module[2].rot.y=0;
	m_Module[2].rot.z=0;
    m_Module[2].JntVar_rot=0; 
    m_Module[2].JntVar_rot1=0; 
	
	//2系变化到3系
	m_Module[3].ModuleIndex=3;
	m_Module[3].trans.x=0.09;
	m_Module[3].trans.y=0;
	m_Module[3].trans.z=0.227;
	m_Module[3].rot.x=0;
	m_Module[3].rot.y=0;
	m_Module[3].rot.z=0;
    m_Module[3].JntVar_rot=0; 
    m_Module[3].JntVar_rot1=40; 
	
	//3系变化到4系
	m_Module[4].ModuleIndex=4;
	m_Module[4].trans.x=-0.079;
	m_Module[4].trans.y=0;
	m_Module[4].trans.z=0.1;
	m_Module[4].rot.x=0;
	m_Module[4].rot.y=0;
	m_Module[4].rot.z=0;
    m_Module[4].JntVar_rot=0; 
    m_Module[4].JntVar_rot1=0; 
	
    //4系变化到5系
	m_Module[5].ModuleIndex=5;
	m_Module[5].trans.x=0.079;
	m_Module[5].trans.y=0;
	m_Module[5].trans.z=0.1965;
	m_Module[5].rot.x=0;
	m_Module[5].rot.y=0;
	m_Module[5].rot.z=0;
    m_Module[5].JntVar_rot=0; 
    m_Module[5].JntVar_rot1=-50; 
	
    //5系变化到6系
	m_Module[6].ModuleIndex=6;
	m_Module[6].trans.x=-0.06825;
	m_Module[6].trans.y=0;
	m_Module[6].trans.z=0.08;
	m_Module[6].rot.x=0;
	m_Module[6].rot.y=0;
	m_Module[6].rot.z=0;
    m_Module[6].JntVar_rot=0; 
    m_Module[6].JntVar_rot1=0; 
	
    //6系变化到7系
	m_Module[7].ModuleIndex=7;
	m_Module[7].trans.x=0.06825;
	m_Module[7].trans.y=0;
	m_Module[7].trans.z=0.1717;
	m_Module[7].rot.x=0;
	m_Module[7].rot.y=0;
	m_Module[7].rot.z=0;
    m_Module[7].JntVar_rot=0; 
    m_Module[7].JntVar_rot1=0; 

	if (COMPILE) {
		AllocConsole();
	}
	
}

CRobotControlDoc::~CRobotControlDoc()  //---乐---析构函数
{
	if (COMPILE) {
		FreeConsole();
	}
	delete [] angelset;
	delete [] jnt;

}

void CRobotControlDoc::assignDoc(CRobotControlDoc *doc) {
	int i = 0;
	for (i = 1; i < 8; i++) {
		this->m_Module[i].JntVar_rot = doc->m_Module[i].JntVar_rot;
		this->jnt[i] = doc->jnt[i];
		this->angelset[i] = doc->angelset[i];
	}
}

BOOL CRobotControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
//************绘制背景色************************//
	int i,j;
	glNewList(BACKGROUND,GL_COMPILE);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor3f(0.5,0.5,0.5);            //定义屏幕颜色
	glBegin(GL_QUADS);                //四边形
	glVertex2f(15,15);
	glVertex2f(-15,15);
	glVertex2f(-15,-15);
	glVertex2f(15,-15); 
	glEnd();
	glColor3f(0.0,0.0,0.0);        //绘制格线
	for ( i=-15;i<=15;i+=3)
	{
		glBegin(GL_LINES);
		glVertex2f(-15,(GLfloat)i);
		glVertex2f(15,(GLfloat)i);
		glEnd();
	}
	for ( j=-15;j<=15;j+=3)
	{
		glBegin(GL_LINES);
		glVertex2f((GLfloat)j,-15);
		glVertex2f((GLfloat)j,15);
		glEnd();
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);       
	glPopMatrix();
	glEndList();
//**********************************************************************//
	/*绘制X-Y-Z坐标系*/
	glPushMatrix();
	glNewList(OGL_AXIS_DLIST,GL_COMPILE);
	glDisable(GL_LIGHTING);
	
	glBegin(GL_LINES);                   // X AXIS STARTS - COLOR RED
	glColor3f(1.0f,0.0f,0.0f);	
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f( 0.2f,0.0f,0.0f);
	glEnd();
	
	glBegin(GL_LINES);                   // 写X字母
			 glVertex3f(0.33f,0.0f,0.06f);
			 glVertex3f( 0.37f,0.0f, 0.0f);
			 glEnd();
			 glBegin(GL_LINES);                   
			 glVertex3f(0.37f,0.0f,0.06f);
			 glVertex3f( 0.33f,0.0f, 0.0f);
			 glEnd();
			 
			 glPushMatrix();
			 glTranslatef(0.2,0.0,0.0);      //X轴端圆锥
			 glRotatef(90,0.0,1.0,0.0);
			 gluCylinder(obj,0.02,0.0,0.1,16,16);
			 glPopMatrix();
			 
			 glBegin(GL_LINES);
			 glColor3f(0.0f, 1.0f, 0.0f);	// Y AXIS STARTS - COLOR GREEN
			 glVertex3f( 0.0f,  0.2f, 0.0f);
			 glVertex3f( 0.0f, 0.0f, 0.0f);
			 glEnd();
			 
			 glBegin(GL_LINES);      // 写Y字母 
			 glVertex3f( -0.03f,  0.33f, 0.08f);
			 glVertex3f( 0.0f, 0.33f, 0.05f);
			 glEnd();
			 glBegin(GL_LINES);       
			 glVertex3f( 0.03f,  0.33f, 0.08f);
			 glVertex3f( 0.0f, 0.33f, 0.05f);
			 glEnd();
			 glBegin(GL_LINES);       
			 glVertex3f( 0.0f,  0.33f, 0.05f);
			 glVertex3f( 0.0f, 0.33f, 0.0f);
			 glEnd();
			 
			 glPushMatrix();
			 glTranslatef(0.0,0.2,0.0);     //Y轴端圆锥
			 glRotatef(-90,1.0,0.0,0.0);       
			 gluCylinder(obj,0.02,0.0,0.1,16,16); 
			 glPopMatrix();
			 
			 glBegin(GL_LINES);
			 glColor3f(0.0f, 0.0f, 1.0f);	// Z AXIS STARTS - COLOR BLUE
			 glVertex3f( 0.0f,  0.0f,  0.2f);
			 glVertex3f( 0.0f,  0.0f, 0.0f);
			 glEnd();
			 glBegin(GL_LINES);      // 写Z字母	
			 glVertex3f( -0.025f,  0.0f,  0.36f);
			 glVertex3f( 0.025f,  0.0f, 0.36f);
			 glEnd();
			 glBegin(GL_LINES);      
			 glVertex3f( 0.025f,  0.0f,  0.36f);
			 glVertex3f( -0.025f,  0.0f, 0.32f);
			 glEnd();
			 glBegin(GL_LINES);      
			 glVertex3f( -0.025f,  0.0f,  0.32f);
			 glVertex3f( 0.025f,  0.0f, 0.32f);
			 glEnd();
			 
			 glPushMatrix();
			 glTranslatef(0.0,0.0,0.2);       //Z轴端圆锥
			 gluCylinder(obj,0.02,0.0,0.1,16,16); 
			 glPopMatrix();
			 
			 glEnable(GL_LIGHTING);
			 glEndList();
    glPopMatrix();


/*	glNewList(OGL_AXIS_DLIST,GL_COMPILE);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glScalef(2.0,2.0,2.0);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);	// X AXIS STARTS - COLOR RED
	glVertex3f(0.0f,  0.0f, 0.0f);
	glVertex3f( 0.2f,  0.0f, 0.0f);
	glVertex3f( 0.2f,  0.0f, 0.0f);	// TOP PIECE OF ARROWHEAD
	glVertex3f( 0.15f,  0.04f, 0.0f);
	glVertex3f( 0.2f,  0.0f, 0.0f);	// BOTTOM PIECE OF ARROWHEAD
	glVertex3f( 0.15f, -0.04f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);	// Y AXIS STARTS - COLOR GREEN
	glVertex3f( 0.0f,  0.2f, 0.0f);
	glVertex3f( 0.0f, 0.0f, 0.0f);			
	glVertex3f( 0.0f,  0.2f, 0.0f);	// TOP PIECE OF ARROWHEAD
	glVertex3f( 0.04f,  0.15f, 0.0f);
	glVertex3f( 0.0f,  0.2f, 0.0f);	// BOTTOM PIECE OF ARROWHEAD
	glVertex3f( -0.04f,  0.15f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);	// Z AXIS STARTS - COLOR BLUE
	glVertex3f( 0.0f,  0.0f,  0.2f);
	glVertex3f( 0.0f,  0.0f, 0.0f);
	glVertex3f( 0.0f,  0.0f, 0.2f);	// TOP PIECE OF ARROWHEAD
	glVertex3f( 0.04f,  0.0f, 0.15f);
	glVertex3f( 0.0f, 0.0f, 0.2f);	// BOTTOM PIECE OF ARROWHEAD
	glVertex3f( -0.04f, 0.0f, 0.15f);
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glEndList();*/

	/////////////////////////////////////////////////////////////////////////
	int k;
    i=0,j=0,k=0;
	fp=fopen("module\\dizuo120.stl","r");  //读取底座
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { prldizuo[i].Normal[0]=a[0];  prldizuo[i].Normal[1]=a[1];  prldizuo[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { prldizuo[i].v1[0]=a[0]*kc;  prldizuo[i].v1[1]=a[1]*kc;  prldizuo[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { prldizuo[i].v2[0]=a[0]*kc;  prldizuo[i].v2[1]=a[1]*kc;  prldizuo[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { prldizuo[i].v3[0]=a[0]*kc;  prldizuo[i].v3[1]=a[1]*kc;  prldizuo[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	prldizuonum=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\prl120.stl","r");  //读取模块prl120
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { prl120[i].Normal[0]=a[0];  prl120[i].Normal[1]=a[1];  prl120[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { prl120[i].v1[0]=a[0]*kc;  prl120[i].v1[1]=a[1]*kc;  prl120[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { prl120[i].v2[0]=a[0]*kc;  prl120[i].v2[1]=a[1]*kc;  prl120[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { prl120[i].v3[0]=a[0]*kc;  prl120[i].v3[1]=a[1]*kc;  prl120[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	prl120num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\prl100.stl","r");  //读取模块prl100
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { prl100[i].Normal[0]=a[0];  prl100[i].Normal[1]=a[1];  prl100[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { prl100[i].v1[0]=a[0]*kc;  prl100[i].v1[1]=a[1]*kc;  prl100[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { prl100[i].v2[0]=a[0]*kc;  prl100[i].v2[1]=a[1]*kc;  prl100[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { prl100[i].v3[0]=a[0]*kc;  prl100[i].v3[1]=a[1]*kc;  prl100[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	prl100num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\prl80.stl","r");  //读取模块prl80
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { prl80[i].Normal[0]=a[0];  prl80[i].Normal[1]=a[1];  prl80[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { prl80[i].v1[0]=a[0]*kc;  prl80[i].v1[1]=a[1]*kc;  prl80[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { prl80[i].v2[0]=a[0]*kc;  prl80[i].v2[1]=a[1]*kc;  prl80[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { prl80[i].v3[0]=a[0]*kc;  prl80[i].v3[1]=a[1]*kc;  prl80[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	prl80num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\prl60.stl","r");  //读取模块prl60
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { prl60[i].Normal[0]=a[0];  prl60[i].Normal[1]=a[1];  prl60[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { prl60[i].v1[0]=a[0]*kc;  prl60[i].v1[1]=a[1]*kc;  prl60[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { prl60[i].v2[0]=a[0]*kc;  prl60[i].v2[1]=a[1]*kc;  prl60[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { prl60[i].v3[0]=a[0]*kc;  prl60[i].v3[1]=a[1]*kc;  prl60[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	prl60num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\lianjie120_120.stl","r");  //读取连接件120_120
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { ver120_120[i].Normal[0]=a[0];  ver120_120[i].Normal[1]=a[1];  ver120_120[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { ver120_120[i].v1[0]=a[0]*kc;  ver120_120[i].v1[1]=a[1]*kc;  ver120_120[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { ver120_120[i].v2[0]=a[0]*kc;  ver120_120[i].v2[1]=a[1]*kc;  ver120_120[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { ver120_120[i].v3[0]=a[0]*kc;  ver120_120[i].v3[1]=a[1]*kc;  ver120_120[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	ver120_120num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\lianjie120_100.stl","r");  //读取连接件120_100
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { ver120_100[i].Normal[0]=a[0];  ver120_100[i].Normal[1]=a[1];  ver120_100[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { ver120_100[i].v1[0]=a[0]*kc;  ver120_100[i].v1[1]=a[1]*kc;  ver120_100[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { ver120_100[i].v2[0]=a[0]*kc;  ver120_100[i].v2[1]=a[1]*kc;  ver120_100[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { ver120_100[i].v3[0]=a[0]*kc;  ver120_100[i].v3[1]=a[1]*kc;  ver120_100[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	ver120_100num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\lianjie100_100.stl","r");  //读取连接件100_100
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { ver100_100[i].Normal[0]=a[0];  ver100_100[i].Normal[1]=a[1];  ver100_100[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { ver100_100[i].v1[0]=a[0]*kc;  ver100_100[i].v1[1]=a[1]*kc;  ver100_100[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { ver100_100[i].v2[0]=a[0]*kc;  ver100_100[i].v2[1]=a[1]*kc;  ver100_100[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { ver100_100[i].v3[0]=a[0]*kc;  ver100_100[i].v3[1]=a[1]*kc;  ver100_100[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	ver100_100num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\lianjie100_80.stl","r");  //读取连接件100_80
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { ver100_80[i].Normal[0]=a[0];  ver100_80[i].Normal[1]=a[1];  ver100_80[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { ver100_80[i].v1[0]=a[0]*kc;  ver100_80[i].v1[1]=a[1]*kc;  ver100_80[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { ver100_80[i].v2[0]=a[0]*kc;  ver100_80[i].v2[1]=a[1]*kc;  ver100_80[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { ver100_80[i].v3[0]=a[0]*kc;  ver100_80[i].v3[1]=a[1]*kc;  ver100_80[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	ver100_80num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\lianjie80_80.stl","r");  //读取连接件80_80
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { ver80_80[i].Normal[0]=a[0];  ver80_80[i].Normal[1]=a[1];  ver80_80[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { ver80_80[i].v1[0]=a[0]*kc;  ver80_80[i].v1[1]=a[1]*kc;  ver80_80[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { ver80_80[i].v2[0]=a[0]*kc;  ver80_80[i].v2[1]=a[1]*kc;  ver80_80[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { ver80_80[i].v3[0]=a[0]*kc;  ver80_80[i].v3[1]=a[1]*kc;  ver80_80[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	ver80_80num=i+1;  //读取的面片数


	i=0;j=0;k=0;
	fp=fopen("module\\lianjie80_60.stl","r");  //读取连接件80_60
	while(fgets(str,128,fp))    
	   {
        sscanf(str,"%s",buf1);
		if(buf1[0]=='f' || buf1[0]=='v') 
		{  sscanf(str,"%*[^-0-9]%f%f%f",a,&a[1],&a[2]); i=k/4; j=k%4; k++;  //k:表示读每行
	       switch(j) {
		   case 0:
			   { ver80_60[i].Normal[0]=a[0];  ver80_60[i].Normal[1]=a[1];  ver80_60[i].Normal[2]=a[2];}
			   break;
		   case 1:
			   { ver80_60[i].v1[0]=a[0]*kc;  ver80_60[i].v1[1]=a[1]*kc;  ver80_60[i].v1[2]=a[2]*kc;}
			   break;
		   case 2:
			   { ver80_60[i].v2[0]=a[0]*kc;  ver80_60[i].v2[1]=a[1]*kc;  ver80_60[i].v2[2]=a[2]*kc;}
			   break;
		   case 3:
			   { ver80_60[i].v3[0]=a[0]*kc;  ver80_60[i].v3[1]=a[1]*kc;  ver80_60[i].v3[2]=a[2]*kc;}
			   break;
		   default:break;
		   }		   
		}  
	}
	fclose(fp);  
	ver80_60num=i+1;  //读取的面片数

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc serialization

void CRobotControlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc diagnostics

#ifdef _DEBUG
void CRobotControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRobotControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc commands

void CRobotControlDoc::RenderScene()
{
    /*glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
    glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,aspect,0.1,200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
	//设置视点
	gluLookAt(0.0,-m_zoom,2.5,0.0,0.0,0.0,0.0,0.0,1.0);  //x正朝右(red),y正朝里(green)
    //使视角可以绕X Z轴旋转
	glRotated(m_xRotation, 1.0, 0.0, 0.0);
    glRotated(m_zRotation, 0.0, 0.0, 1.0); 
	glScalef(2.0,2.0,2.0);
	glCallList(BACKGROUND);               //Draw background
	glPushMatrix();
	glRotated(-90,0,0,1);
	glTranslated(0,0,0.31);
	glCallList(OGL_AXIS_DLIST);
	glPopMatrix();
	if(Lineartestflag==true)
	{
	glPushMatrix();
	glRotated(-90,0,0,1);
	glTranslated(0,0,0.31);
//	glCallList(OGL_AXIS_DLIST);
	glTranslated(ini_px,ini_py,ini_pz);
	glRotated(ini_aif,0.0,0.0,1.0);
	glRotated(ini_bit,0.0,1.0,0.0);
	glRotated(ini_gam,1.0,0.0,0.0);
	glScaled(0.5f,0.5f,0.5f);
	glCallList(OGL_AXIS_DLIST);  //Draw x-y-z轴
	glPopMatrix();	
	glPushMatrix();
	glRotated(-90,0,0,1);
	glTranslated(0,0,0.31);
	glTranslated(des_px,des_py,des_pz);
	glRotated(des_aif,0.0,0.0,1.0);
	glRotated(des_bit,0.0,1.0,0.0);
	glRotated(des_gam,1.0,0.0,0.0);
	glScaled(0.5f,0.5f,0.5f);
	glCallList(OGL_AXIS_DLIST);  //Draw x-y-z轴
	glPopMatrix();	
	glPushMatrix();
	glRotated(-90,0,0,1);
	glTranslated(0,0,0.31);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,1.0);
	glVertex3d(ini_px,ini_py,ini_pz);
	glVertex3d(des_px,des_py,des_pz);
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix(); 
	}

	if(AutoPlanFlag==true)    //末端轨迹跟踪
	{	
		glPushMatrix();
		glRotated(-90,0,0,1);
		glTranslated(0,0,0.31);
		glTranslated(PreObj[0],PreObj[1],PreObj[2]);
		glRotated(des_aif,0.0,0.0,1.0);
		glRotated(des_bit,0.0,1.0,0.0);
		glRotated(des_gam,1.0,0.0,0.0);
       	glScaled(0.5f,0.5f,0.5f);
		glCallList(OGL_AXIS_DLIST);  //Draw x-y-z轴
		glPopMatrix();	 
		glPushMatrix();
		glRotated(-90,0,0,1);
		glTranslated(0,0,0.31);
		glTranslated(des_px,des_py,des_pz);
		glRotated(des_aif,0.0,0.0,1.0);
		glRotated(des_bit,0.0,1.0,0.0);
		glRotated(des_gam,1.0,0.0,0.0);
       	glScaled(0.5f,0.5f,0.5f);
		glCallList(OGL_AXIS_DLIST);  //Draw x-y-z轴
		glPopMatrix();			
		glPushMatrix();
		glRotated(-90,0,0,1);
		glTranslated(0,0,0.31);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0,0.0,1.0);
		glVertex3d(ini_px,ini_py,ini_pz);
		glVertex3d(PreObj[0],PreObj[1],PreObj[2]); 
		glVertex3d(des_px,des_py,des_pz);
		glEnd();
		glEnable(GL_LIGHTING);
		glPopMatrix(); 
	}   

	DrawBase();
	
	glPushMatrix();
	glTranslatef(0.0,0.0,0.31);
	DrawModule(0);

	// added by gjx
	DrawGoal();
	for(int i=1; i<8; i++)  
	{
		glRotated(m_Module[i].rot.z, 0.0f, 0.0f, 1.0f);
		glTranslated(0.0f,0.0f,m_Module[i].trans.z);
		glRotated(m_Module[i].rot.y, 0.0f, 1.0f, 0.0f);
		glTranslated(0.0f,m_Module[i].trans.y,0.0f);
        glRotated(m_Module[i].rot.x, 1.0f, 0.0f, 0.0f);
       	glTranslated(m_Module[i].trans.x,0.0,0.0);
		DrawModule(i);   
    } 
	DrawInfo();

	
	glPopMatrix();
	
	
	
}

void CRobotControlDoc::DrawModule(int index)
{
	int i;
	switch(index)
	{
	case 0: //底座
	glTranslated(m_Module[0].JntVar_trans,0,0);
	glPushMatrix();
	glTranslatef(0.0,0.0,-0.02);
//	glCallList(OGL_AXIS_DLIST);
	glColor3f(0.8,0.8,0.8 );
	auxSolidBox(0.4,1.12,0.04);    //底板

	glRotated(-90,0,0,1);
	glTranslatef(0,0,0.02);
	glCallList(OGL_AXIS_DLIST);
//	glTranslated(0.0,0.02,0.0);
//	glRotated(-90,0,0,1);
	glRotated(90,1,0,0);
	
	glColor3f(0.8,0.8,0.8 );
	for(i=0; i<prldizuonum; i++)
	{	
		glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
		glNormal3fv(prldizuo[i].Normal);
		glVertex3fv(prldizuo[i].v1);
		glVertex3fv(prldizuo[i].v2);
		glVertex3fv(prldizuo[i].v3);
		glEnd();
	}
//	glRotated(180, 1, 0, 0);

	glTranslated(0,0.042,0);
//	glTranslatef(0, -0.155, 0);
//	glCallList(OGL_AXIS_DLIST);
	glColor3f(0.8,0.8,0.8);
	
	for(i=0; i<prl120num; i++)
	{	
		glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
		glNormal3fv(prl120[i].Normal);
		glVertex3fv(prl120[i].v1);
		glVertex3fv(prl120[i].v2);
		glVertex3fv(prl120[i].v3);
		glEnd();
	}
	glPopMatrix();
	break;
	case 1:
		glRotated(m_Module[1].JntVar_rot,0,0,1);
		glPushMatrix();
		
//		glRotated(-90,1,0,0);
//		glRotated(90,0,1,0);
		glRotated(180,1,0,0);
		glRotated(90,0,0,-1);
		glTranslated(0,0.09,-0.11);
//		glRotated(90,0,0,-1);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8 );
		for(i=0; i<ver120_120num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(ver120_120[i].Normal);
			glVertex3fv(ver120_120[i].v1);
			glVertex3fv(ver120_120[i].v2);
			glVertex3fv(ver120_120[i].v3);
			glEnd();
		}

		
//		glTranslatef(0,0,0.096);
//		glTranslatef(0,-0.086,0);
//		glRotated(-90,1,0,0);
		glRotated(180,0,0,1);
		glTranslated(0,0.008,0);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8 );

		for(i=0; i<prl120num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(prl120[i].Normal);
			glVertex3fv(prl120[i].v1);
			glVertex3fv(prl120[i].v2);
			glVertex3fv(prl120[i].v3);
			glEnd();
		}
		glPopMatrix();

		break;
	case 2:
		glRotated(m_Module[2].JntVar_rot,1,0,0);
		glPushMatrix();
		glRotated(90,0,0,1);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8 );
		for(i=0; i<ver120_100num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(ver120_100[i].Normal);
			glVertex3fv(ver120_100[i].v1);
			glVertex3fv(ver120_100[i].v2);
			glVertex3fv(ver120_100[i].v3);
			glEnd();
		}

		glTranslatef(0,-0.09,0.1089-0.008);
		glRotated(90,1,0,0);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<prl100num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(prl100[i].Normal);
			glVertex3fv(prl100[i].v1);
			glVertex3fv(prl100[i].v2);
			glVertex3fv(prl100[i].v3);
			glEnd();
		}
		glPopMatrix();
		break;
	case 3:
		glRotated(m_Module[3].JntVar_rot,0,0,1);
		glPushMatrix();

		glRotated(90,0,0,1);
		glRotated(180,0,1,0);
		glRotated(180,0,0,1);
		glTranslatef(0,0.079,-0.1);

//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<ver100_100num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(ver100_100[i].Normal);
			glVertex3fv(ver100_100[i].v1);
			glVertex3fv(ver100_100[i].v2);
			glVertex3fv(ver100_100[i].v3);
			glEnd();
		}

		glRotated(180,0,0,1);
		glTranslated(0,0.02-0.008,0);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<prl100num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(prl100[i].Normal);
			glVertex3fv(prl100[i].v1);
			glVertex3fv(prl100[i].v2);
			glVertex3fv(prl100[i].v3);
			glEnd();
		}
		glPopMatrix();
		break;
	case 4:
		glRotated(m_Module[4].JntVar_rot,1,0,0);
		glPushMatrix();

		glRotated(90,0,0,1);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<ver100_80num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(ver100_80[i].Normal);
			glVertex3fv(ver100_80[i].v1);
			glVertex3fv(ver100_80[i].v2);
			glVertex3fv(ver100_80[i].v3);
			glEnd();
		}

		glTranslatef(0,-0.079,0.1-0.008);
		glRotated(90,1,0,0);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<prl80num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(prl80[i].Normal);
			glVertex3fv(prl80[i].v1);
			glVertex3fv(prl80[i].v2);
			glVertex3fv(prl80[i].v3);
			glEnd();
		}

		glPopMatrix();
		break;
	case 5:
		glRotated(m_Module[5].JntVar_rot,0,0,1);
		glPushMatrix();

		glRotated(90,0,0,1);
		glRotated(180,1,0,0);
		glTranslatef(0.0,0.06825,-0.08);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<ver80_80num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(ver80_80[i].Normal);
			glVertex3fv(ver80_80[i].v1);
			glVertex3fv(ver80_80[i].v2);
			glVertex3fv(ver80_80[i].v3);
			glEnd();
		}
		
		glRotated(180,1,0,0);
		glTranslatef(0,0.02-0.008,0);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<prl80num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(prl80[i].Normal);
			glVertex3fv(prl80[i].v1);
			glVertex3fv(prl80[i].v2);
			glVertex3fv(prl80[i].v3);
			glEnd();
		}
		glPopMatrix();
		break;
	case 6:
		glRotated(m_Module[6].JntVar_rot,1,0,0);
		glPushMatrix();

		glRotated(90,0,0,1);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<ver80_60num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(ver80_60[i].Normal);
			glVertex3fv(ver80_60[i].v1);
			glVertex3fv(ver80_60[i].v2);
			glVertex3fv(ver80_60[i].v3);
			glEnd();
		}

		glTranslatef(0,-0.06825,0.08-0.008);
		glRotated(90,1,0,0);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		for(i=0; i<prl60num; i++)
		{	
			glBegin(GL_TRIANGLES);                   // X AXIS STARTS - COLOR RED
			glNormal3fv(prl60[i].Normal);
			glVertex3fv(prl60[i].v1);
			glVertex3fv(prl60[i].v2);
			glVertex3fv(prl60[i].v3);
			glEnd();
		}

		glPopMatrix();
		break;
	case 7:
		glRotated(m_Module[7].JntVar_rot,0,0,1);
		glPushMatrix();
//		glTranslatef(-0.01,0,0);、
		glRotated(-90,0,0,1);
//		glCallList(OGL_AXIS_DLIST);
		glColor3f(0.8,0.8,0.8);
		gluCylinder(obj,0.02,0.02,0.1,20,1);
		glTranslatef(0,0,0.1);
		glCallList(OGL_AXIS_DLIST);
		glPopMatrix();
		break;

	default:break;
	}

}

void CRobotControlDoc::DrawBase()
{
	glPushMatrix();  //此时，坐标系为视基坐标系,即x(red,右),y(green,里),z(blue,上),原点(0,0,0)
	glColor3f(0.5f,0.5f,0.5f);  
	glTranslatef(0.0f,0.50f,0.135f);  
	auxSolidBox(3.9f,0.12f,0.27f);  //后导轨
    glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f,-0.50f,0.135f);  
	auxSolidBox(3.9f,0.12f,0.27f);  //前导轨
    glPopMatrix(); 

}

void CRobotControlDoc::DrawInfo()
{
	glPushMatrix();
	glColor3ub(255, 224, 224);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, 100 , 0.0,100, -1.0, 1.0);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);  

	CString strTemp;
	int i=3,j;

	double m_VelShoulder = 0.0;
	double m_VelElbow = 0.0;
	double m_VelWrist = 0.0;
    double m_dMubiao[6]={-1540, 4650, 8450, 0, 0, 0};
	glColor3f(0.8,0.1,0.8);

    strTemp.Format("目标位置  %7.3f   %7.3f   %7.3f   %7.3f   %7.3f   %7.3f",
	des_px, des_py, des_pz, des_aif, des_bit, des_gam);
	PrintfBitmap(strTemp,3,70+10*i,0);

	glColor3f(0.2,0.2,0.2);
	strTemp.Format("关节编号   关节位置   关节速度    标号     末端位姿   末端速度");
	PrintfBitmap(strTemp,3,70+9*i,0);
  
	glColor3f(0.8,0.1,0.8);
	strTemp.Format("   0     %7.3f    %7.3f", jnt[0], JntW[0]);
	PrintfBitmap(strTemp,3, 70+8*i,0);
	strTemp.Format("   1     %7.3f    %7.3f", jnt[1], JntW[1]);
	PrintfBitmap(strTemp,3, 70+7*i,0);
	strTemp.Format("   2     %7.3f    %7.3f", jnt[2], JntW[2]);
	PrintfBitmap(strTemp,3, 70+6*i,0);
	strTemp.Format("   3     %7.3f    %7.3f", jnt[3], JntW[3]);
	PrintfBitmap(strTemp,3, 70+5*i,0);
	strTemp.Format("   4     %7.3f    %7.3f", jnt[4], JntW[4]);
	PrintfBitmap(strTemp,3, 70+4*i,0);
	strTemp.Format("   5     %7.3f    %7.3f", jnt[5], JntW[5]);
	PrintfBitmap(strTemp,3, 70+3*i,0);
	strTemp.Format("   6     %7.3f    %7.3f", jnt[6], JntW[6]);
	PrintfBitmap(strTemp,3, 70+2*i,0);
	strTemp.Format("   7     %7.3f    %7.3f", jnt[7], JntW[7]);
	PrintfBitmap(strTemp,3, 70+1*i,0);

	
	////纯平，非宽屏显示
    
    /////////////////////////末端位姿+速度
	strTemp.Format("%7.3f    %7.3f", EndPE[0], EndVW[0]);
	PrintfBitmap(strTemp,28, 70+7*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[1], EndVW[1]);
	PrintfBitmap(strTemp,28, 70+6*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[2], EndVW[2]);
	PrintfBitmap(strTemp,28, 70+5*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[3], EndVW[3]);
	PrintfBitmap(strTemp,28, 70+4*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[4], EndVW[4]);
	PrintfBitmap(strTemp,28, 70+3*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[5], EndVW[5]);
	PrintfBitmap(strTemp,28, 70+2*i,0);
    


	/////宽屏显示
/*  
    /////////////////////////末端位姿+速度
	strTemp.Format("%7.3f    %7.3f", EndPE[0], EndVW[0]);
	m_font.PrintfBitmap(strTemp,63, 70+7*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[1], EndVW[1]);
	m_font.PrintfBitmap(strTemp,63, 70+6*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[2], EndVW[2]);
	m_font.PrintfBitmap(strTemp,63, 70+5*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[3], EndVW[3]);
	m_font.PrintfBitmap(strTemp,63, 70+4*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[4], EndVW[4]);
	m_font.PrintfBitmap(strTemp,63, 70+3*i,0);
	strTemp.Format("%7.3f    %7.3f", EndPE[5], EndVW[5]);
	m_font.PrintfBitmap(strTemp,63, 70+2*i,0);
*/
	glColor3f(0.2,0.2,0.2);


//	strTemp.Format("末端位置： %7.3f  %7.3f  %7.3f  %7.3f  %7.3f  %7.3f",Pos_x,Pos_y,Pos_z,Eul_z,Eul_y,Eul_x);
//	m_font.PrintfBitmap(strTemp,3, 70+1*i,0);

	glColor3f(0.4,0.0,0.2);

 //   if(planendflag==false)
//	strTemp.Format("总运行时间： %10.3f       当前时间：   %10.3f", m_time, No*t0);
//	strTemp.Format("总运行时间： %10.3f       当前时间：   %10.3f", m_time, numtime*t0);
//	else
//	strTemp.Format("总运行时间： %10.3f       当前时间：   %10.3f", m_time, m_time);
//	PrintfBitmap(strTemp,3, 70+0*i,0); 
 /*
	if(SinguStateflag==0)
	strTemp.Format("奇异状态：    机械臂未奇异");
	else
	strTemp.Format("奇异状态：    机械臂奇异!");
	m_font.PrintfBitmap(strTemp,3, 70-1*i,0);

*/	

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST); 
	glPopMatrix();

}

void CRobotControlDoc::DrawGoal() {
	if (drawGoalFlag) {
		if (COMPILE) {
			_cprintf("Here begin to draw the cube in model view.\n");
		}
		// to draw the object here, use a cube to describe it
		glPushMatrix();
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		// draw cube
		glTranslated(goalPos[1], -goalPos[0], goalPos[2]);
		glColor3f(0.0, 0.0, 1.0);
		glutWireCube(0.3f);
		glutSolidSphere(0.05f, 50, 50);
		// draw center point
		glPointSize(0.3);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(goalPos[1], -goalPos[0], goalPos[2]);
		glEnd();
		CString strTemp;
		strTemp.Format("%5.3f    %5.3f    %5.3f", goalPos[0], goalPos[1], goalPos[2]);
		PrintfBitmap(strTemp, goalPos[0], goalPos[1], goalPos[2]);
		glPopMatrix();
		if (COMPILE) {
			_cprintf("Here end drawing the cube and center point.\n");
		}
	}
}

void CRobotControlDoc::PrintfBitmap( LPCTSTR lpszText, GLfloat x, GLfloat y, GLfloat z)
{
	CBitmap mfcBitmap;  //位图数据
	BITMAP bm;
	SIZE size;
	UCHAR* pBmpBits; //位图像素位
	HFONT hOldFont;  //旧字体指针
	HBITMAP hPrevBmp;
	
	HDC hdc = ::wglGetCurrentDC(); //获得当前设备场景
	
	//if(!m_hFont) m_hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);		// use default system gui font
	//if(!m_hFont) m_hFont = (HFONT)::GetStockObject(SYSTEM_FONT);		// use default system font
	
	// 选择新字体,保存旧字体
	//hOldFont = (HFONT)SelectObject(hdc, m_hFont); 
	
	::GetTextExtentPoint32(hdc, lpszText, strlen(lpszText), &size);
	
	// 创建字符位图对象(对应字符位图空间)
	mfcBitmap.CreateBitmap(size.cx, size.cy, 1, 1, NULL);
	
	// 创建与hdc兼容的暂存DC
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	
	if(hMemDC)
	{   //选择字符位图到暂存DC
		hPrevBmp = (HBITMAP)SelectObject(hMemDC,mfcBitmap);
	}
	
	HFONT hPrevFont = (HFONT)SelectObject(hMemDC, m_hFont);
	
	SetBkColor(hMemDC, RGB(0, 0, 0));
	SetTextColor(hMemDC, RGB(255, 255, 255));
	SetBkMode(hMemDC, OPAQUE);
	
	// 输出文字到暂存DC
	TextOut(hMemDC, 0, 0, lpszText, strlen(lpszText));
	
	// copy GDI bitmap to DIB
	mfcBitmap.GetBitmap(&bm); //获得字符位图数据
	
	size.cx = (bm.bmWidth + 31) & (~31);
	size.cy = bm.bmHeight;
	
	int bufsize = size.cy * (size.cx /8);
	
	pBmpBits = new UCHAR[bufsize];
	memset(pBmpBits, 0, sizeof(UCHAR)*bufsize);
	
	struct
	{
		BITMAPINFOHEADER bih;
		RGBQUAD col[2];
	}bic;
	
	BITMAPINFO *binf = (BITMAPINFO *)&bic;
	binf->bmiHeader.biSize = sizeof(binf->bmiHeader);
	binf->bmiHeader.biWidth = bm.bmWidth;
	binf->bmiHeader.biHeight = bm.bmHeight;
	binf->bmiHeader.biPlanes = 1;
	binf->bmiHeader.biBitCount = 1;
	binf->bmiHeader.biCompression = BI_RGB;
	binf->bmiHeader.biSizeImage = bufsize;
	binf->bmiHeader.biXPelsPerMeter = 1;
	binf->bmiHeader.biYPelsPerMeter = 1;
	binf->bmiHeader.biClrUsed = 0;
	binf->bmiHeader.biClrImportant = 0;
	
	::GetDIBits(hdc, mfcBitmap, 0, bm.bmHeight, pBmpBits, binf,DIB_RGB_COLORS);
	::SelectObject(hMemDC, hPrevBmp);
	::DeleteDC(hMemDC);
	
	//::SelectObject(hdc, hOldFont);  // 设回旧字体
	
	::glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //控制像素存储
	::glRasterPos3f(x,y,z);                  //
	::glBitmap(size.cx, size.cy, 0.0, 0.0, size.cx, 0.0, pBmpBits); //写位图到缓存
	
	delete pBmpBits;
}