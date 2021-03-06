// ntfsdumpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ntfsdump.h"
#include "ntfsdumpDlg.h"

#include "../../NTFSLib/NTFS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNtfsdumpDlg dialog

CNtfsdumpDlg::CNtfsdumpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNtfsdumpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNtfsdumpDlg)
	m_filename = _T("");
	m_dump = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNtfsdumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNtfsdumpDlg)
	DDX_Text(pDX, IDC_FILENAME, m_filename);
	DDX_Text(pDX, IDE_DUMP, m_dump);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNtfsdumpDlg, CDialog)
	//{{AFX_MSG_MAP(CNtfsdumpDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNtfsdumpDlg message handlers

BOOL CNtfsdumpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNtfsdumpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNtfsdumpDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNtfsdumpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// ugly but work !
void ShowData(CString &m_dump, BYTE *data, DWORD datalen)
{
	// "0000    01 02 03 04 05 06 07 08 - 09 0A 0B 0C 0D 0E 0F   123456789ABCDEF";

	if (datalen == 0)
		return;

	CString line;
	BYTE *p;
	DWORD i;

	for (i=0; i<((datalen-1)>>4); i++)
	{
		p = data + i*16;

		line.Format(_T("%04X    %02X %02X %02X %02X %02X %02X %02X %02X - %02X %02X %02X %02X %02X %02X %02X %02X   "),
			i*16, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
		for (int j=0; j<16; j++)
		{
			if (p[j] < 0x20)
				p[j] = '.';
		}
		line.Format(_T("%s%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n"),
			line, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);

		m_dump += line;
	}

	// last line
	p = data + i*16;
	BYTE q[16];
	memset(q, 0xFF, 16);
	memcpy(q, p, datalen-i*16);
	line.Format(_T("%04X    %02X %02X %02X %02X %02X %02X %02X %02X - %02X %02X %02X %02X %02X %02X %02X %02X   "),
		i*16, q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8], q[9], q[10], q[11], q[12], q[13], q[14], q[15]);
	for (int j=0; j<16; j++)
	{
			if (q[j] < 0x20)
			q[j] = '.';
	}
	line.Format(_T("%s%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"),
		line, q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8], q[9], q[10], q[11], q[12], q[13], q[14], q[15]);
	m_dump += line;
}

void CNtfsdumpDlg::OnOK() 
{
	CFileDialog fd(TRUE);

	if (fd.DoModal() == IDOK)
	{
		m_filename = fd.GetPathName();
		m_dump.Empty();
		UpdateData(FALSE);

		// parse volume

		_TCHAR volname = m_filename.GetAt(0);

		CNTFSVolume volume(volname);
		if (!volume.IsVolumeOK())
		{
			MessageBox(_T("Not a valid NTFS volume or NTFS version < 3.0"));
			return;
		}

		// parse root directory

		CFileRecord fr(&volume);
		// we only need to parse INDEX_ROOT and INDEX_ALLOCATION
		// don't waste time and ram to parse unwanted attributes
		fr.SetAttrMask(MASK_INDEX_ROOT | MASK_INDEX_ALLOCATION);

		if (!fr.ParseFileRecord(MFT_IDX_ROOT))
		{
			MessageBox(_T("Cannot read root directory of volume"));
			return;
		}

		if (!fr.ParseAttrs())
		{
			MessageBox(_T("Cannot parse attributes"));
			return;
		}

		// find subdirectory

		CIndexEntry ie;

		int dirs = m_filename.Find(_T('\\'), 0);
		int dire = m_filename.Find(_T('\\'), dirs+1);
		while (dire != -1)
		{
			CString pathname = m_filename.Mid(dirs+1, dire-dirs-1);

			if (fr.FindSubEntry((const _TCHAR*)pathname, ie))
			{
				if (!fr.ParseFileRecord(ie.GetFileReference()))
				{
					MessageBox(_T("Cannot read root directory of volume"));
					return;
				}

				if (!fr.ParseAttrs())
				{
					if (fr.IsCompressed())
						MessageBox(_T("Compressed directory not supported yet"));
					else if (fr.IsEncrypted())
						MessageBox(_T("Encrypted directory not supported yet"));
					else
						MessageBox(_T("Cannot parse directory attributes"));
					return;
				}
			}
			else
			{
				MessageBox(_T("File not found\n"));
				return;
			}

			dirs = dire;
			dire = m_filename.Find(_T('\\'), dirs+1);
		}

		// dump it !

		CString filename = m_filename.Right(m_filename.GetLength()-dirs-1);
		if (fr.FindSubEntry((const _TCHAR*)filename, ie))
		{
			if (!fr.ParseFileRecord(ie.GetFileReference()))
			{
				MessageBox(_T("Cannot read file"));
				return;
			}

			// We only need DATA attribute and StdInfo
			fr.SetAttrMask(MASK_DATA);
			if (!fr.ParseAttrs())
			{
				if (fr.IsCompressed())
					MessageBox(_T("Compressed file not supported yet"));
				else if (fr.IsEncrypted())
					MessageBox(_T("Encrypted file not supported yet"));
				else
					MessageBox(_T("Cannot parse file attributes"));
				return;
			}

			BYTE *filebuf = new BYTE[16*1024];

			// only pick the unnamed stream (file data)
			const CAttrBase *data = fr.FindStream();
			if (data)
			{
				DWORD datalen = (DWORD)data->GetDataSize();
				if (datalen > 16*1024)
					datalen = 16*1024;	// show only the first 16K

				DWORD len;
				if (data->ReadData(0, filebuf, datalen, &len) && len == datalen)
				{
					ShowData(m_dump, filebuf, datalen);
					UpdateData(FALSE);
				}
				else
				{
					MessageBox(_T("Read data error"));
					return;
				}
			}

			delete filebuf;
		}
		else
		{
			MessageBox(_T("File not found\n"));
			return;
		}
	}
}
