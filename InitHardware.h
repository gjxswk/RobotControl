#if !defined(AFX_INITHARDWARE_H__EC2B31BE_B7B3_4BF6_9FD9_54EA30CD5858__INCLUDED_)
#define AFX_INITHARDWARE_H__EC2B31BE_B7B3_4BF6_9FD9_54EA30CD5858__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InitHardware.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInitHardware dialog
typedef long (WINAPI *DOWNLOADGETPROC)(long, LPTSTR, long);
typedef void (WINAPI *DOWNLOADPROGRESS)(long);
typedef void (WINAPI *DOWNLOADMSGPROC)(LPTSTR,BOOL);
typedef struct _PAMAMETER
{
	float DefHomeOffset;
	float DefMinPos;
	float DefMaxPos;
	float DefMaxDeltaPos;
	//	float DefMaxDeltaVel;
	//	float DefMinVel;
	float DefMaxVel;
	//	float DefMinAcc;
	float DefMaxACC;
	//	float DefMinCur;
	float DefMaxCur;
	float DefHomeVel;
	float ActPos;
	float ActVel;
	float DeltaPos;
	//	float DefHomeAcc;
	unsigned long DefSerialNo;
	unsigned short DefCubeVersion;
	unsigned char ModuleType;
	unsigned long DefConfig;
	unsigned long ModuleState;

	int m_CubeID;
	bool m_bModuleSelected;
	
}Parameter;
extern "C" _declspec(dllimport) BOOL _stdcall OpenPmacDevice(DWORD dwDevice);
extern "C" _declspec(dllimport) BOOL _stdcall ClosePmacDevice(DWORD dwDevice);
extern "C" _declspec(dllimport) BOOL _stdcall PmacGetResponseExA(DWORD dwDevice,PCHAR s,UINT maxchar,PCHAR outstr);
extern "C" _declspec(dllimport) BOOL _stdcall PmacGetResponseA(DWORD dwDevice,PCHAR s,UINT maxchar,PCHAR outstr);
extern "C" _declspec(dllimport) BOOL _stdcall PmacGetBufferA(DWORD, PCHAR, UINT);
extern "C" _declspec(dllimport) BOOL _stdcall InBufferedMode(DWORD);
extern "C" _declspec(dllimport) long _stdcall PmacDownloadA(DWORD, DOWNLOADMSGPROC,DOWNLOADGETPROC, DOWNLOADPROGRESS,PCHAR, BOOL, BOOL, BOOL, BOOL);
extern "C" _declspec(dllimport) void _stdcall PmacSetIVariableDouble(DWORD dwDevice,UINT num,DOUBLE val);
extern "C" _declspec(dllimport) void _stdcall PmacGetIVariableDouble(DWORD dwDevice,UINT num,DOUBLE def);
class CInitHardware : public CDialog
{
// Construction
public:
	CInitHardware(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInitHardware)
	enum { IDD = IDD_INITHARDWARE };
	CStatic	m_CDeltaPos;
	CStatic	m_CActVel;
	CStatic	m_CActPos;
	CListCtrl	m_ListCtrl;
	CString	m_mail;
	float	m_velocity;
	float	m_absoposition;
	float	m_relaposition;
	float	m_PosModule1;
	float	m_PosModule2;
	float	m_PosModule3;
	float	m_VelModule1;
	float	m_VelModule2;
	float	m_VelModule3;
	float	m_VelModule4;
	float	m_VelModule5;
	float	m_VelModule6;
	float	m_PosModule4;
	float	m_PosModule5;
	float	m_PosModule6;
	float	m_AccModule1;
	float	m_AccModule2;
	float	m_AccModule3;
	float	m_AccModule4;
	float	m_AccModule5;
	float	m_AccModule6;
	float	m_VelModule7;
	float	m_PosModule7;
	float	m_AccModule7;
	CString	m_ActPos;
	CString	m_ActVel;
	CString	m_DeltaPos;
	//}}AFX_DATA

public:
	int PmacstateValue;
	long dwDevice;
	TCHAR buf[256];
	Parameter Parameter[31];
	int m_device;
	int m_intCount;
	HANDLE m_hThread;
	int ModuleSelect;
	int PMacdevice;
//	int MotionMode;

public:
	static int Scanning();
	void errorBeep();
	void FillStatusCol( int i );
	float GetRadFromDegree( float deg );
	float GetDegreeFromRad( float rad);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitHardware)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInitHardware)
	afx_msg void OnButtonPmacopen();
	afx_msg void OnButtonPmacclose();
	afx_msg void OnButtonZero();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonMakesure();
	afx_msg void OnButtonRun1();
	afx_msg void OnButtonRun2();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonScan();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInitmodule();
	afx_msg void OnResetmodule();
	afx_msg void OnHaltmodule();
	afx_msg void OnClickListModule(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonExit();
	afx_msg void OnButtonGo1();
	afx_msg void OnButtonGo2();
	afx_msg void OnButtonGo3();
	afx_msg void OnButtonGo4();
	afx_msg void OnButtonGo5();
	afx_msg void OnButtonGo6();
	afx_msg void OnButtonGo7();
	afx_msg void OnRadioModule1();
	afx_msg void OnRadioModule2();
	afx_msg void OnRadioModule3();
	afx_msg void OnRadioModule4();
	afx_msg void OnRadioModule5();
	afx_msg void OnRadioModule6();
	afx_msg void OnRadioModule7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticActpos();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITHARDWARE_H__EC2B31BE_B7B3_4BF6_9FD9_54EA30CD5858__INCLUDED_)
