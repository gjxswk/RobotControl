// DataShow.cpp : 实现文件
//

#include "stdafx.h"
#include "RobotControl.h"
#include "DataShow.h"
#include "afxdialogex.h"
#include "resource.h"
#include "afxext.h"


// DataShow 对话框

IMPLEMENT_DYNAMIC(DataShow, CDialog)

DataShow::DataShow(CWnd* pParent /*=NULL*/)
	: CDialog(DataShow::IDD, pParent)
{
	
}

DataShow::~DataShow()
{
}

void DataShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DataShow, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// DataShow 消息处理程序
int DataShow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	return 0;
}