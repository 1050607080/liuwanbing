
// LKFishToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LKFishTool.h"
#include "LKFishToolDlg.h"
#include "afxdialogex.h"
#include "ResUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLKFishToolDlg �Ի���




CLKFishToolDlg::CLKFishToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLKFishToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLKFishToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLKFishToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CLKFishToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLKFishToolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CLKFishToolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CLKFishToolDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CLKFishToolDlg ��Ϣ�������

BOOL CLKFishToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLKFishToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLKFishToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLKFishToolDlg::OnBnClickedButton1()
{
	this->GetDlgItemTextW(IDC_MFCEDITBROWSE2,m_strSource);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	{
		CString dwjlog;
		dwjlog.Format(L"dwjlog::m_strSource=%s",m_strSource);
		OutputDebugString(dwjlog);
	}
	if (m_strSource == "")
	{
		MessageBox(L"�����ļ�Ϊ��",L"��ʾ",0);
		return;
	}
	if (EncryptFishFile(m_strSource, true))
	{
		MessageBox(L"�����ļ����",L"��ʾ",0);
	}	
}
void CLKFishToolDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->GetDlgItemTextW(IDC_MFCEDITBROWSE2,m_strSource);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	{
		CString dwjlog;
		dwjlog.Format(L"dwjlog::m_strSource=%s",m_strSource);
		OutputDebugString(dwjlog);
	}
	if (m_strSource == "")
	{
		MessageBox(L"�����ļ�Ϊ��",L"��ʾ",0);
		return;
	}
	if (EncryptFishFile(m_strSource, false))
	{
		MessageBox(L"�����ļ����",L"��ʾ",0);
	}
	
}




void	CLKFishToolDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->GetDlgItemTextW(IDC_MFCEDITBROWSE2,m_strSource);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	{
		CString dwjlog;
		dwjlog.Format(L"dwjlog::m_strSource=%s",m_strSource);
		OutputDebugString(dwjlog);
	}
	if (m_strSource == "")
	{
		MessageBox(L"�����ļ�Ϊ��",L"��ʾ",0);
		return;
	}
	if (DecryptFishFile(m_strSource, true))
	{
		MessageBox(L"�����ļ����",L"��ʾ",0);
	}
}

void CLKFishToolDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->GetDlgItemTextW(IDC_MFCEDITBROWSE2,m_strSource);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	{
		CString dwjlog;
		dwjlog.Format(L"dwjlog::m_strSource=%s",m_strSource);
		OutputDebugString(dwjlog);
	}
	if (m_strSource == "")
	{
		MessageBox(L"�����ļ�Ϊ��",L"��ʾ",0);
		return;
	}
	if (DecryptFishFile(m_strSource, false))
	{
		MessageBox(L"�����ļ����",L"��ʾ",0);
	}	
}



bool	CLKFishToolDlg::EncryptFishFile(const CString& strFile, BOOL bDeleteSourceFile)
{
	FILE* file = NULL;
	_tfopen_s(&file, strFile, _T("rb"));
	if (file == NULL) {
		MessageBox(_T("�ļ���ʧ��"));
		return false;
	}

	int pos = strFile.ReverseFind(_T('.'));
	if (pos < 0)
	{
		MessageBox(_T("�ļ�������"));
		return false;
	}
	CString strDstFile = strFile.Left(pos);
	strDstFile += _T(".fish");

	fseek(file, 0, SEEK_END);
	long file_len = ftell(file);

	fseek(file, 0, SEEK_SET);
	BYTE* file_data = new BYTE[file_len];
	fread(file_data, file_len, 1, file);
	for (long i = 0; i < file_len; ++i)
	{
		file_data[i] = g_EncryptByteMap[file_data[i]];
	}

	// ��������
	DWORD dwXorKey = g_dwKey;
	WORD* pwSeed = (WORD*)file_data;
	DWORD* pdwXor = (DWORD*)file_data;
	WORD wEncrypCount = (WORD)(file_len / sizeof(DWORD));
	for (WORD i = 0; i < wEncrypCount; i++)
	{
		*pdwXor++ ^= dwXorKey;
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwKey;
	}

	FILE* wfile = NULL;
	_tfopen_s(&wfile, strDstFile, _T("wb"));
	fwrite(file_data, file_len, 1, wfile);

	delete[] file_data;
	fclose(file);
	fclose(wfile);

	if (bDeleteSourceFile)
	{
		DeleteFile(strFile);
	}

	return true;
}




bool	CLKFishToolDlg::DecryptFishFile(const CString& strFile, BOOL bDeleteSourceFile)
{
	FILE* file = NULL;
	_tfopen_s(&file, strFile, _T("rb"));
	if (file == NULL) 
	{
		MessageBox(_T("�ļ���ʧ��"));
		return false;
	}

	int pos = strFile.ReverseFind(_T('.'));
	if (pos < 0)
	{
		MessageBox(_T("�ļ�������"));
		return false;
	}
	CString strDstFile = strFile.Left(pos);

	fseek(file, 0, SEEK_END);
	long file_len = ftell(file);

	fseek(file, 0, SEEK_SET);
	BYTE* file_data = new BYTE[file_len];
	fread(file_data, file_len, 1, file);

	// ��������
	DWORD dwXorKey = g_dwKey;
	DWORD dwDecryptKey = g_dwKey;
	WORD* pwSeed = (WORD*)file_data;
	DWORD* pdwXor = (DWORD*)file_data;
	WORD wDecrypCount = (WORD)(file_len / sizeof(DWORD));
	for (WORD i = 0; i < wDecrypCount; i++)
	{
		dwXorKey = SeedRandMap(*pwSeed++);
		dwXorKey |= ((DWORD)SeedRandMap(*pwSeed++)) << 16;
		dwXorKey ^= g_dwKey;
		*pdwXor++ ^= dwDecryptKey;
		dwDecryptKey = dwXorKey;
	}

	for (long i = 0; i < file_len; ++i)
	{
		file_data[i] = g_DecryptByteMap[file_data[i]];
	}

	strDstFile += GetFileExtension(file_data);

	FILE* wfile = NULL;
	_tfopen_s(&wfile, strDstFile, _T("wb"));
	fwrite(file_data, file_len, 1, wfile);

	delete[] file_data;
	fclose(file);
	fclose(wfile);

	if (bDeleteSourceFile)
	{
		DeleteFile(strFile);
	}

	return true;
}
