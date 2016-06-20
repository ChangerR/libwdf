// maptoolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "maptool.h"
#include "maptoolDlg.h"
#include "XY.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CmaptoolDlg �Ի���




CmaptoolDlg::CmaptoolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CmaptoolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmaptoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PACK, m_Pack_Button);
	DDX_Control(pDX, IDC_UNPACK, m_UnPack_Button);
	DDX_Control(pDX, IDC_GAME_PATH, m_GamePath);
	DDX_Control(pDX, IDC_FILE_PATH, m_FilePath);
	DDX_Control(pDX, IDC_CHECK1, m_IsBig);
	DDX_Control(pDX, IDC_CHECK5, m_IsSmall);
	DDX_Control(pDX, IDC_CHECK6, m_IsMask);
	DDX_Control(pDX, IDC_CHECK7, m_IsCell);
	DDX_Control(pDX, IDC_CHECK2, m_IsFileExist);
}

BEGIN_MESSAGE_MAP(CmaptoolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PACK, &CmaptoolDlg::OnBnClickedPack)
	ON_BN_CLICKED(IDC_UNPACK, &CmaptoolDlg::OnBnClickedUnpack)
	ON_BN_CLICKED(IDC_GAME_BROWSE, &CmaptoolDlg::OnBnClickedGameBrowse)
	ON_BN_CLICKED(IDC_FILE_BROWSE, &CmaptoolDlg::OnBnClickedFileBrowse)
END_MESSAGE_MAP()


// CmaptoolDlg ��Ϣ�������

BOOL CmaptoolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_IsBig.SetCheck(TRUE);
	m_IsSmall.SetCheck(FALSE);
	m_IsMask.SetCheck(FALSE);
	m_IsCell.SetCheck(FALSE);
	m_IsFileExist.SetCheck(TRUE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CmaptoolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmaptoolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CmaptoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmaptoolDlg::OnBnClickedPack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(L"����汾���޴˹���",L"��ʾ",0);
}

void CmaptoolDlg::OnBnClickedUnpack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_UnPack_Button.EnableWindow(FALSE); // ���ð�ť
	CXY *xy=new CXY();

	BOOL b1 = m_IsBig.GetCheck();
	BOOL b2 = m_IsSmall.GetCheck();
	BOOL b3 = m_IsMask.GetCheck();
	BOOL b4 = m_IsCell.GetCheck();
	BOOL b5 = m_IsFileExist.GetCheck();

	xy->SetIsBig(b1);
	xy->SetIsSmall(b2);
	xy->SetIsMask(b3);
	xy->SetIsCell(b4);
	xy->SetIsFileExist(b5);

	m_GamePath.GetWindowTextW(m_sGamePath);
	m_FilePath.GetWindowTextW(m_sFilePath);
	if(m_sGamePath=="")
	{
		MessageBox(L"��ѡ����Ϸ��װĿ¼��·��",L"��ʾ",0);
		m_UnPack_Button.EnableWindow(TRUE); // ���ð�ť
		return;
	}
	if(m_sFilePath=="")
	{
		MessageBox(L"��ѡ������ļ�Ŀ¼��·��",L"��ʾ",0);
		m_UnPack_Button.EnableWindow(TRUE); // ���ð�ť
		return;
	}


	int type = xy->GetGameType(m_sGamePath);
	if(type==0) 
	{
		MessageBox(L"��ѡ����ȷ����Ϸ��װĿ¼��·��\nδ�ҵ���ϷĿ¼�µ�Update.ini�ļ�",L"��ʾ",0);
		m_UnPack_Button.EnableWindow(TRUE); // ���ð�ť
		return;
	}
	CStringList* FileList = xy->GetFileList(m_sGamePath); // ����ļ��б�

	// ѭ����*.map�ļ���������
	POSITION pos = FileList->GetHeadPosition();
	POSITION pos1 = xy->GetFileName()->GetHeadPosition();
	while (pos!=NULL)
	{
		CString path = FileList->GetNext(pos);
		CString name = xy->GetFileName()->GetNext(pos1);
		xy->OpenMap(path,name,m_sFilePath);
	}
	//xy->~CXY();
	delete xy;

	m_UnPack_Button.EnableWindow(TRUE); // ���ð�ť
}

void CmaptoolDlg::OnBnClickedGameBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi; // �������Ϣ
	memset(&bi, 0, sizeof(BROWSEINFO));   

	char Buffer[MAX_PATH]="";

	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot =NULL;//��ʼ���ƶ���rootĿ¼�ܲ����ף�
	//bi.pszDisplayName = *Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = L"��ѡ����������ϵ�а�װĿ¼��";
	//bi.ulFlags = BIF_BROWSEINCLUDEFILES;//�����ļ�
	bi.ulFlags = BIF_EDITBOX;//�����ļ�

	bi.lpfn = NULL;
	bi.iImage=IDR_MAINFRAME;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, (LPWSTR)&Buffer); //ȡ���ļ���·����Buffer��
	}
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	m_GamePath.SetWindowTextW((LPCTSTR)Buffer);
}

void CmaptoolDlg::OnBnClickedFileBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BROWSEINFO bi; // �������Ϣ
	memset(&bi, 0, sizeof(BROWSEINFO));   

	char Buffer[MAX_PATH]="";

	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot =NULL;//��ʼ���ƶ���rootĿ¼�ܲ����ף�
	//bi.pszDisplayName = *Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = L"��ѡ������ļ���Ŀ¼��";
	//bi.ulFlags = BIF_BROWSEINCLUDEFILES;//�����ļ�
	bi.ulFlags = BIF_EDITBOX;//�����ļ�

	bi.lpfn = NULL;
	bi.iImage=IDR_MAINFRAME;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, (LPWSTR)&Buffer); //ȡ���ļ���·����Buffer��
	}
	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//�ͷ��ڴ�
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
	m_FilePath.SetWindowTextW((LPCTSTR)Buffer);
}
