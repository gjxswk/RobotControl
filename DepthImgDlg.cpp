// DepthImgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RobotControl.h"
#include "DepthImgDlg.h"
#include "afxdialogex.h"


// DepthImgDlg �Ի���

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


// DepthImgDlg ��Ϣ�������




void DepthImgDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
