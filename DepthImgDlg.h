#pragma once


// DepthImgDlg 对话框

class DepthImgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DepthImgDlg)

public:
	DepthImgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DepthImgDlg();

// 对话框数据
	enum { IDD = IDD_DepthImg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
