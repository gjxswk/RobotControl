#pragma once


// DataShow 对话框

class DataShow : public CDialog
{
	DECLARE_DYNAMIC(DataShow)

public:
	DataShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DataShow();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATASHOW };

protected:
	//{{AFX_MSG(CSideBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
