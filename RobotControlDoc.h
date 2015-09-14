// RobotControlDoc.h : interface of the CRobotControlDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include "Datatype.h"
#include "Dynamic_spacerobot.h"

#if !defined(AFX_ROBOTCONTROLDOC_H__AB27F144_DE28_46F1_94D6_05F53DAD6AFF__INCLUDED_)
#define AFX_ROBOTCONTROLDOC_H__AB27F144_DE28_46F1_94D6_05F53DAD6AFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRobotControlDoc : public CDocument
{
//protected: // create from serialization only
public:	// redefined by gjx
	CRobotControlDoc();
	DECLARE_DYNCREATE(CRobotControlDoc)
public:
	int JOINT_NUM;
	GLdouble m_zRotation;
	GLdouble m_xRotation;
	GLdouble m_zoom;
    t_Module m_Module[10];
	GLdouble aspect;
//	CGLFont m_font;                     // 字体
	float ini_px,ini_py,ini_pz,des_px,des_py,des_pz;
	float ini_aif,ini_bit,ini_gam,des_aif,des_bit,des_gam;
	float *angelset;///存储初始关节变量
	double spx,spy,spz, spaif, spbit, spgam;//目标物位姿
	//float jnt0, jnt1,jnt2,jnt3,jnt4,jnt5,jnt6,jnt7;//随关节变化的关节角度（弧度）
	float *jnt;
	int No;//规划步数
	int numtime;//当前步数规划时间
	double Pn[Ni][3];
	double EndPE[6], EndVW[6], BasPE[6], BasVW[6], JntW[8]; //末端位姿/速度，基座位姿/速度，关节速度
	double PreObj[3];
	double goalPos[6]; // 目标点末端伴娘姿

	bool Lineartestflag;
    bool HandEyeFlag;
	BOOL AutoPlanFlag;
	int CapOrientFlag;
	bool drawGoalFlag;
// Attributes
public:
	  void RenderScene();
	  void DrawModule(int index);
	  void DrawBase();
	  void DrawInfo();
	  void DrawGoal();
	  void assignDoc(CRobotControlDoc *doc);
	  void PrintfBitmap( LPCTSTR lpszText, GLfloat x, GLfloat y, GLfloat z=0);
	  GLUquadricObj* obj;
	  HFONT m_hFont; //字体句柄


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotControlDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRobotControlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRobotControlDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTCONTROLDOC_H__AB27F144_DE28_46F1_94D6_05F53DAD6AFF__INCLUDED_)
