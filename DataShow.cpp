// DataShow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RobotControl.h"
#include "DataShow.h"
#include "afxdialogex.h"
#include "resource.h"
#include "afxext.h"


// DataShow �Ի���

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


// DataShow ��Ϣ�������
int DataShow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	return 0;
}