// SerialIODlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SerialIO.h"
#include "SerialIODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SHOT_PRO_ECHO "*SHOTPRO %s/STA:%d/CTB:22.000/UH:555.5\r\n"
#define SHOT_PRO_GPS_ECHO "*SHOTPRO %s/STA:%d/CTB:22.000/UH:555.5\n$GPGGA, 161050, 3645.1926,N,09705.0707,W,2,06,1,+0321.0,M,,M,,\r\n"

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// CSerialIODlg 对话框
CSerialIODlg::CSerialIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialIODlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, output_edit_);
	DDX_Control(pDX, IDC_COMBO1, combobox_);
	DDX_Control(pDX, IDOK, button_);
	DDX_Control(pDX, IDC_CHECK1, gps_check_);
}

BEGIN_MESSAGE_MAP(CSerialIODlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSerialIODlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSerialIODlg 消息处理程序

BOOL CSerialIODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	be_startup_ = false;
	combobox_.AddString("0: 未点火");
	combobox_.AddString("1: 接受到以点火放炮和状态,一切正常");
	combobox_.AddString("2: 未接受到状态(电台故障)");
	combobox_.AddString("3: 接收到状态,但没有井口模拟数据(电台故障)");
	combobox_.AddString("4: 译码器电池电量太低警告");
	combobox_.AddString("5: 井口检波器电阻未测量或超出允许范围");
	combobox_.AddString("6: 雷管阻抗未测试或超出允许范围");
	combobox_.AddString("7: 自动井口时间拾取不成功");
	combobox_.SetCurSel(1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSerialIODlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSerialIODlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSerialIODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerialIODlg::output_log(CString const& log)
{
	int nBegin, nEnd;
	CString str, str_text;

	if(log.GetLength() == 0) return;

	if(output_edit_.LineIndex(1) != -1){

		nBegin = output_edit_.LineIndex(0);
		nEnd = nBegin + output_edit_.LineLength(nBegin); 
		output_edit_.GetLine(0, str_text.GetBuffer(nEnd), nEnd);
		str_text.ReleaseBuffer(nEnd);
		output_edit_.SetSel(nBegin, nEnd);
		str.Format(_T("%s\r\n%s"), log, str_text);
		output_edit_.ReplaceSel(str);
	}
	else{
		str.Format(_T("%s\r\n"), log);
		output_edit_.SetWindowText(str);
	}
}

void CSerialIODlg::com_output(CString const& result_text)
{
	char lpOutBuffer[512];
	CString tmp = result_text;
	DWORD dwBytesWrite = result_text.GetLength();
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	BOOL bWriteStat;

	ClearCommError(com_port_, &dwErrorFlags, &ComStat);
	strcpy(lpOutBuffer, tmp.GetBuffer());
	tmp.ReleaseBuffer();
	bWriteStat = WriteFile(com_port_, lpOutBuffer, dwBytesWrite, &dwBytesWrite, NULL);

	if(!bWriteStat)	AfxMessageBox("写串口失败!");

	PurgeComm(com_port_, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
}

void CSerialIODlg::parse_and_output_info(CString const& info)
{
	CString tmp;
	int index = 0;

	tmp = info;
	tmp.Replace("*SP#", "炮号: ");
	tmp.Replace("/SL#", "  震源点测线: ");
	tmp.Replace("/SN#", "  震源点号: ");
	tmp.Replace("/SI#", "  震源点索引: ");
	tmp.Replace("/SEQ#", "  爆炸机号: ");

	output_log(tmp);
}

void CSerialIODlg::echo_status(CString const& info)
{
	int index = 0;
	CString tmp;
	CString data;
	int check_status = 0x0;

	check_status = gps_check_.GetCheck();
	index = combobox_.GetCurSel();

	tmp = info.Mid(1, info.GetLength() - 1);

	if(check_status == BST_CHECKED)			data.Format(SHOT_PRO_GPS_ECHO, tmp, index);
	else if(check_status == BST_UNCHECKED)	data.Format(SHOT_PRO_ECHO, tmp, index);
	
	com_output(data);
	output_log(data);
}

void CSerialIODlg::OnTimer(UINT_PTR nIDEvent)
{
	char str[256];
	memset(str, 0x0,sizeof(str));
	DWORD wCount=256;
	BOOL bReadStat;
	int index = 0x0;
	CString data, tmp;
	BLASTER_PROTOCOL br;

	memset(&br, 0x0, sizeof(br));

	bReadStat = ReadFile(com_port_, str, wCount, &wCount, NULL);
	if(bReadStat != 0 && wCount != 0){

		PurgeComm(com_port_, PURGE_TXABORT| PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		recv_info_ += str;
		while((index = recv_info_.Find("\r\n")) != -1){//"\r\n"

			data = recv_info_.Mid(0, index);
			tmp = recv_info_.Mid(index + 2, recv_info_.GetLength() - index - 2);//2
			recv_info_ = tmp;
			//分解 & 输出
			parse_and_output_info(data);
			//返回状态
			echo_status(data);
		}		
	}
//	else	AfxMessageBox("读串口失败!");

}

void CSerialIODlg::OnBnClickedOk()
{
	DCB dcb;
	COMMTIMEOUTS com_timeouts;

	memset(&dcb, 0x0, sizeof(dcb));
	memset(&com_timeouts, 0x0, sizeof(com_timeouts));

	if(!be_startup_){

		//create com1 file
		com_port_ = CreateFile("\\\\.\\COM1",	GENERIC_READ|GENERIC_WRITE,	0, NULL, OPEN_EXISTING, 0, NULL);
		if(com_port_ == INVALID_HANDLE_VALUE){
			AfxMessageBox("打开COM失败!");
			return;
		}

		SetupComm(com_port_, 256, 256);
		com_timeouts.ReadIntervalTimeout = MAXDWORD;
		com_timeouts.ReadTotalTimeoutMultiplier=0;
		com_timeouts.ReadTotalTimeoutConstant=0;

		com_timeouts.WriteTotalTimeoutMultiplier=128;
		com_timeouts.WriteTotalTimeoutConstant=500;
		SetCommTimeouts(com_port_,&com_timeouts); 

		dcb.DCBlength = sizeof(dcb);
		dcb.BaudRate = CBR_9600;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		
		SetCommState(com_port_, &dcb);
		PurgeComm(com_port_, PURGE_RXCLEAR | PURGE_TXCLEAR);

		recv_info_ = _T("");
		button_.SetWindowText("结束");
		SetTimer(1, 1000,NULL);
		be_startup_ = true;
	}
	else{
		be_startup_ = false;
		KillTimer(1);
		button_.SetWindowText("开始");

		CloseHandle(com_port_);
		com_port_ = NULL;
	}
}
