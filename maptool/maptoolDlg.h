// maptoolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CmaptoolDlg �Ի���
class CmaptoolDlg : public CDialog
{
// ����
public:
	CmaptoolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAPTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPack();
	CButton m_Pack_Button;
	CButton m_UnPack_Button;
	CString m_sGamePath; // ȫ����Ϸ��װĿ¼·�� 
	CString m_sFilePath; // ȫ�����Ŀ¼·��
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
