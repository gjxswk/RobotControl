// DataShow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RobotControl.h"
#include "DataShow.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// DataShow ��Ϣ�������
