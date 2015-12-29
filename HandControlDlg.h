#if !defined(AFX_HANDCONTROLDLG_H__4F3DCCE1_B915_482A_89B0_5FAF9F859A16__INCLUDED_)
#define AFX_HANDCONTROLDLG_H__4F3DCCE1_B915_482A_89B0_5FAF9F859A16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HandControlDlg.h : header file
//
#include "RobotControlDoc.h"
#include "RobotControlView.h"
/////////////////////////////////////////////////////////////////////////////
// CHandControlDlg dialog


class CHandControlDlg : public CDialog
{
// Construction
public:
	CHandControlDlg(CWnd* pParent = NULL);   // standard constructor
	CRobotControlView* view;
	CRobotControlDoc* doc;
	void SendMessageToJXB();
// Dialog Data
	//{{AFX_DATA(CHandControlDlg)
	enum { IDD = IDD_HANDCONTROL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHandControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

public:
	int kn,No, numtime;
	CString str1,str2,str3,str4,str5,str6,str7;
	void ShowJntVariable();
	void qmtomModule();
//	DWORD WINAPI HandControl(LPVOID dlg);
	double ini_ang[8], des_ang[8], now_ang[8], now_PEca[6];
    float t06[4][4];
	float oula[6];
    int Mission_Number;
	double EndPE[Ni][6];
	unsigned char ColStatusWord;
	unsigned char CorFlag, HandleFlag, MotionFlag;
	float ScrValue;
	double VelValue;
	double next_ang[8], next_angvel[8], next_baspe[6], next_basvel[6];
	double now_baspe[6], now_angvel[8],former_ang[8],former_baspe[6],former_angvel[8];
	int ControlMode;
	double VelCts;//导轨速度
	double PosCts;//导轨绝对位置
	TCHAR buf[255];
	int PMacdevice;
	//{{AFX_MSG(CHandControlDlg)
	afx_msg void OnRadioTranslate();
	afx_msg void OnRadioRotate();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonXtransp();
	afx_msg void OnButtonYtransp();
	afx_msg void OnButtonXtransi();
	afx_msg void OnButtonYtransi();
	afx_msg void OnButtonZtransp();
	afx_msg void OnButtonZtransi();
	afx_msg void OnButtonXrotp();
	afx_msg void OnButtonXroti();
	afx_msg void OnButtonYrotp();
	afx_msg void OnButtonYroti();
	afx_msg void OnButtonZrotp();
	afx_msg void OnButtonZroti();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckRemotecontrol();
	afx_msg void OnButtonStoph();
	afx_msg void OnButtonReseth();
	afx_msg void OnButtonSeth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HANDCONTROLDLG_H__4F3DCCE1_B915_482A_89B0_5FAF9F859A16__INCLUDED_)
