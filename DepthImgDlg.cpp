// DepthImgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RobotControl.h"
#include "DepthImgDlg.h"
#include "afxdialogex.h"


// DepthImgDlg 对话框

IMPLEMENT_DYNAMIC(DepthImgDlg, CDialogEx)

DepthImgDlg::DepthImgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(DepthImgDlg::IDD, pParent)
{
}

DepthImgDlg::~DepthImgDlg()
{
}

void DepthImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DepthImgDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &DepthImgDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// DepthImgDlg 消息处理程序




void DepthImgDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
