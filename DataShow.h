#pragma once


// DataShow �Ի���

class DataShow : public CDialog
{
	DECLARE_DYNAMIC(DataShow)

public:
	DataShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DataShow();

// �Ի�������
	enum { IDD = IDD_DIALOG_DATASHOW };

protected:
	//{{AFX_MSG(CSideBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
