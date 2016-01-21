#if !defined(AFX_PATHPLANDLG_H__CEFC6C93_09D3_4C06_B3B8_D8B5616F390D__INCLUDED_)
#define AFX_PATHPLANDLG_H__CEFC6C93_09D3_4C06_B3B8_D8B5616F390D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathPlanDlg.h : header file
//
#include "RobotControlView.h"
#include "LinerPlanSetDlg.h"
#include "AutoPlanSetDlg.h"
#include "HandControlDlg.h"
#include "DepthImgDlg.h"
#include "cvvimage.h"
#include "math.h"
#include "DataShow.h"
#include <bitset>
#include <list>
//#include "cv.h"
//#include "highgui.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "VirtualScene.h"

#include <opencv2/opencv.hpp>
//#include "MainFrm.h"
//#include "RobotControlDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CPathPlanDlg dialog

class CPathPlanDlg : public CDialog
{
// Construction
public:
	CPathPlanDlg(CWnd* pParent = NULL);   // standard constructor
	~CPathPlanDlg();
	CRobotControlView* view;
	CRobotControlMultiDoc* multi_doc;
	CRobotControlDoc* doc;
	CRobotControlDoc* doc_real;
	CLinerPlanSetDlg* pLinerPlanSetDlg;
	CAutoPlanSetDlg* pAutoPlanSetDlg;
	CHandControlDlg* pHandControlDlg;
	DepthImgDlg* pDepthImgDlg;
	DataShow* dataShowDlg;
	void SendMessageToJXB();
//	friend class CMainFrame;
// Dialog Data
	//{{AFX_DATA(CPathPlanDlg)
	enum { IDD = IDD_PATHPLAN };
	int JOINT_NUM;
	float   *m_JNT;
	double	m_ts;
	int		m_time;
	float	m_dpx;
	float	m_dpy;
	float	m_dpz;
	float	m_daif;
	float	m_dbit;
	float	m_dgam;
	float	m_ini_j0;
	float	m_ini_j1;
	float	m_ini_j2;
	float	m_ini_j3;
	float	m_ini_j4;
	float	m_ini_j5;
	float	m_ini_j6;
	float	m_ini_j7;
	float	m_ini_j8;
	float	m_baspx;
	float	m_baspy;
	float	m_baspz;
	float	m_basaif;
	float	m_basbit;
	float	m_basgam;
	double CtsVel;
	
	float predict_time;
	float predict_time_error;

	double *former_angle;
	double *des_angle;
	double *des_vel;
	double *des_acc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathPlanDlg)
	public:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam); 
		void viewProc(UINT message, WPARAM wParam, LPARAM lParam);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int kn,No;//kn�滮�ܲ�����NO��ǰ�滮����
	int Planradio;//�滮ģʽ��־λ
	int PreProgrammFlag;//Ԥ��̱�־λ
	int ControlMode;//ģʽ��־λ
	int numtime;//��ǰ�滮ʱ��
	int CapOrientFlag;
	double GivenObj[6];
	double PreObj[3];
	float ceta[Ni][8];
	double VelCts;//�����ٶ�
	double PosCts;//�������λ��
	TCHAR buf[255];
	int PMacdevice;
	clock_t start;
	clock_t end;
	clock_t start_linear;
	int ctrl_mode;
	bool btn_release;


	void ShowJntVariable();
	void qmtomModule();
	bool deviceIsHalt();
	int isNear(double a, double b);
	bool simFinishMoving();

//	void InitialSettingData();
	
	// Generated message map functions
	//{{AFX_MSG(CPathPlanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonSet();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonInit();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRadioLinearplan();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonIniok();
	afx_msg void OnButtonRun();
	afx_msg void OnRadioAutoplan();
	afx_msg void OnButtonStop();
	afx_msg void OnRadioHandcontrol();
	afx_msg void OnRadioSumu();
	afx_msg void OnRadioJxb();
	afx_msg void OnButtonSavedata();
	afx_msg void OnRadioProgramm();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void linearMove(int direction);
	void DrawPicToHDC(IplImage* img, unsigned int ID, int mode);
	void Array_Multipy(float inv_T[4][4], float c[4], float multipy[4]);
	void getCurrentJointAngel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonKeyControl();
	afx_msg void OnButtonCamera();
	afx_msg void OnButtonCatchMode();
	afx_msg void OnButtonShutCamera();
	afx_msg void OnButtonDataShow();
};

/************not use the MATLAB *****************/
//DWORD WINAPI  OnBnClickedButtonGo(LPVOID lpParameter);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHPLANDLG_H__CEFC6C93_09D3_4C06_B3B8_D8B5616F390D__INCLUDED_)
