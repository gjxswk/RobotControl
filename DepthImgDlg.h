#pragma once


// DepthImgDlg �Ի���

class DepthImgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DepthImgDlg)

public:
	DepthImgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DepthImgDlg();

// �Ի�������
	enum { IDD = IDD_DepthImg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
