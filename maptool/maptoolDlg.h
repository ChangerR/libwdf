// maptoolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CmaptoolDlg 对话框
class CmaptoolDlg : public CDialog
{
// 构造
public:
	CmaptoolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAPTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPack();
	CButton m_Pack_Button;
	CButton m_UnPack_Button;
	CString m_sGamePath; // 全局游戏安装目录路径 
	CString m_sFilePath; // 全局输出目录路径
	afx_msg void OnBnClickedUnpack();
	afx_msg void OnBnClickedGameBrowse();
	afx_msg void OnBnClickedFileBrowse();
	CEdit m_GamePath;
	CEdit m_FilePath;
	CButton m_IsBig;
	CButton m_IsSmall;
	CButton m_IsMask;
	CButton m_IsCell;
	CButton m_IsFileExist;
};
