// UartToUdpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UartToUdp.h"
#include "UartToUdpDlg.h"
#include "UDPSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CUartToUdpDlg 对话框




CUartToUdpDlg::CUartToUdpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUartToUdpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CUartToUdpDlg::~CUartToUdpDlg()
{
	
	m_pUDPSocket->Close();
	if (m_pUDPSocket != NULL)
	{
		m_pUDPSocket = NULL;
		delete m_pUDPSocket;
	}
	
	if (m_uiPortNb != NULL)
	{
		m_uiPortNb = NULL;
		delete m_uiPortNb;
	}
	
}
void CUartToUdpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERIALPORT_BAUD, m_comboSerialPortBaud);
	DDX_Control(pDX, IDC_COMBO_SERIALPORT_COM, m_comboSerialPortCom);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlMSComm1);
}

BEGIN_MESSAGE_MAP(CUartToUdpDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPENSERIALPORT, &CUartToUdpDlg::OnBnClickedButtonOpenserialport)
END_MESSAGE_MAP()


// CUartToUdpDlg 消息处理程序

BOOL CUartToUdpDlg::OnInitDialog()
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

	//端口设置
	m_comboSerialPortCom.SetWindowText( _T(COM0) );		// 设置控件初始化显示
	m_comboSerialPortCom.InsertString( 0, _T(COM0) );	// 插入下拉菜单选项
	m_comboSerialPortCom.InsertString( 1, _T(COM1) );
	m_comboSerialPortCom.InsertString( 2, _T(COM2) );
	m_comboSerialPortCom.InsertString( 3, _T(COM3) );
	m_comboSerialPortCom.InsertString( 4, _T(COM4) );
	m_comboSerialPortCom.InsertString( 5, _T(COM5) );
	m_comboSerialPortCom.InsertString( 6, _T(COM6) );
	m_comboSerialPortCom.InsertString( 7, _T(COM7) );
	m_comboSerialPortCom.InsertString( 8, _T(COM8) );

	//波特率设置
	m_comboSerialPortBaud.SetWindowText( _T(BAUD11) );	// 设置控件初始化显示
	m_comboSerialPortBaud.InsertString( 0, _T(BAUD0) );	// 插入下拉菜单选项
	m_comboSerialPortBaud.InsertString( 1, _T(BAUD1) );
	m_comboSerialPortBaud.InsertString( 2, _T(BAUD2) );
	m_comboSerialPortBaud.InsertString( 3, _T(BAUD3) );
	m_comboSerialPortBaud.InsertString( 4, _T(BAUD4) );
	m_comboSerialPortBaud.InsertString( 5, _T(BAUD5) );
	m_comboSerialPortBaud.InsertString( 6, _T(BAUD6) );
	m_comboSerialPortBaud.InsertString( 7, _T(BAUD7) );
	m_comboSerialPortBaud.InsertString( 8, _T(BAUD8) );
	m_comboSerialPortBaud.InsertString( 9, _T(BAUD9) );
	m_comboSerialPortBaud.InsertString( 10, _T(BAUD10) );
	m_comboSerialPortBaud.InsertString( 11, _T(BAUD11) );

	// 串口通讯设置
	m_ctrlMSComm1.put_InBufferCount(0);		// 清空接收缓存
	m_ctrlMSComm1.put_OutBufferCount(0);	// 清空发送缓存
	m_ctrlMSComm1.put_InputMode(1);         // 二进制模式

	// 创建本地接收端口的SOCKET并设置接收缓冲区大小
	
	m_uiPortNb = new unsigned short [UDPPortNb];
	m_uiPortNb[0] = 0x9002;
	m_uiPortNb[1] = 0x7000;
	m_uiPortNb[2] = 0x8000;
	m_uiPortNb[3] = 0x9000;
	m_uiPortNb[4] = 0x9100;
	m_uiPortNb[5] = 0x9200;
	m_uiPortNb[6] = 0x9300;
	m_uiPortNb[7] = 0x9400;
	m_uiPortNb[8] = 0x9500;
	m_uiPortNb[9] = 0x9600;
	m_uiPortNb[10] = 0x9700;
	m_uiPortNb[11] = 0x6000;
	m_uiPortNb[12] = 0x6200;
	m_uiPortNb[13] = 0x6100;
	m_uiPortNb[14] = 0x6400;
	m_uiPortNb[15] = 0x6600;
	m_uiPortNb[16] = 0x6300;
	m_uiPortNb[17] = 0x6700;

	m_pUDPSocket = new CUDPSocket(this);
	m_pUDPSocket->Create(ReceivePort,SOCK_DGRAM);
	m_pUDPSocket->SetSockOpt(NULL,&m_pUDPSocket,65536,SOL_SOCKET);

	uart_count = 0;
	icount = 0;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUartToUdpDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUartToUdpDlg::OnPaint()
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
HCURSOR CUartToUdpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 得到当前选择的串口号
int CUartToUdpDlg::OnGetSerialPortCom(void)
{
	int iCurrentCom = 0;
	CString str;
	m_comboSerialPortCom.GetWindowText(str);	
	if (str == _T(COM0))
	{
		iCurrentCom = 1;
	}
	else if (str == _T(COM1))
	{
		iCurrentCom = 2;
	}
	else if (str == _T(COM2))
	{
		iCurrentCom = 3;
	}
	else if (str == _T(COM3))
	{
		iCurrentCom = 4;
	}
	else if (str == _T(COM4))
	{
		iCurrentCom = 5;
	}
	else if (str == _T(COM5))
	{
		iCurrentCom = 6;
	}
	else if (str == _T(COM6))
	{
		iCurrentCom = 7;
	}
	else if (str == _T(COM7))
	{
		iCurrentCom = 8;
	}
	else if (str == _T(COM8))
	{
		iCurrentCom = 9;
	}
	else
	{
		AfxMessageBox(_T("Input Serial Port Com Nb is Wrong!"));
	}
	return iCurrentCom;
}

// 得到当前选择的波特率
int CUartToUdpDlg::OnGetSerialPortBaud(void)
{
	int iCurrentBaud = 0;
	CString str;
	m_comboSerialPortBaud.GetWindowText(str);	
	if (str == _T(BAUD0))
	{
		iCurrentBaud = 300;
	}
	else if (str == _T(BAUD1))
	{
		iCurrentBaud = 600;
	}
	else if (str == _T(BAUD2))
	{
		iCurrentBaud = 1200;
	}
	else if (str == _T(BAUD3))
	{
		iCurrentBaud = 2400;
	}
	else if (str == _T(BAUD4))
	{
		iCurrentBaud = 4800;
	}
	else if (str == _T(BAUD5))
	{
		iCurrentBaud = 9600;
	}
	else if (str == _T(BAUD6))
	{
		iCurrentBaud = 19200;
	}
	else if (str == _T(BAUD7))
	{
		iCurrentBaud = 38400;
	}
	else if (str == _T(BAUD8))
	{
		iCurrentBaud = 43000;
	}
	else if (str == _T(BAUD9))
	{
		iCurrentBaud = 56000;
	}
	else if (str == _T(BAUD10))
	{
		iCurrentBaud = 57600;
	}	
	else if (str == _T(BAUD11))
	{
		iCurrentBaud = 115200;
	}
	else
	{
		AfxMessageBox(_T("Input Serial Port Baud is Wrong!"));
	}

	return iCurrentBaud;
}

void CUartToUdpDlg::OnBnClickedButtonOpenserialport()
{
	// 先关闭已经打开的端口
	if(m_ctrlMSComm1.get_PortOpen())
	{
		m_ctrlMSComm1.put_PortOpen(FALSE);
	}

	int iPort;
	int iBaud;
	CString str;
	iPort = OnGetSerialPortCom();
	iBaud = OnGetSerialPortBaud();
	m_ctrlMSComm1.put_CommPort(iPort);
	str.Format(_T("%d,n,8,1"), iBaud);
	m_ctrlMSComm1.put_Settings(str);
	
	if(!m_ctrlMSComm1.get_PortOpen())
	{
		m_ctrlMSComm1.put_PortOpen(TRUE);
		AfxMessageBox(_T("Open Serial Port Successful!"));
	}
}
BEGIN_EVENTSINK_MAP(CUartToUdpDlg, CDialog)
	ON_EVENT(CUartToUdpDlg, IDC_MSCOMM1, 1, CUartToUdpDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

void CUartToUdpDlg::OnCommMscomm1()
{
	VARIANT vResponse;
	int len;
	unsigned char* Rxdata = NULL;
	if(m_ctrlMSComm1.get_CommEvent()==2)
	{
		len = m_ctrlMSComm1.get_InBufferCount(); //接收到的字符数目

		if(len > 0) 
		{
			vResponse=m_ctrlMSComm1.get_Input(); //read
			Rxdata = (unsigned char*) vResponse.parray->pvData;
			if (len >1024)
			{
				AfxMessageBox(_T("Receive data from Uart leaked!"));
				return;
			} 
			
			for (int i=0; i<len; i++)
			{
				switch(icount)
				{
				case 0:
					if (Rxdata[i] == 0x11)
					{
						uart_buf[uart_count][0] = Rxdata[i];
						icount++;
						break;
					}
					else
					{
						icount = 0;
						break;
					}

				case 1:
					if (Rxdata[i] == 0x22)
					{
						uart_buf[uart_count][1] = Rxdata[i];
						icount++;
						break;
					}
					else
					{
						icount = 0;
						break;
					}
				case 2:
					if (Rxdata[i] == 0x33)
					{
						uart_buf[uart_count][2] = Rxdata[i];
						icount++;
						break;
					}
					else
					{
						icount = 0;
						break;
					}
				case 3:
					if (Rxdata[i] == 0x44)
					{
						uart_buf[uart_count][3] = Rxdata[i];
						icount++;
						break;
					}
					else
					{
						icount = 0;
						break;
					}
				default:
					uart_buf[uart_count][icount] = Rxdata[i];
					icount++;
					if (icount == 256)
					{
						icount = 0;
						OnSendFromUDP(uart_buf[uart_count], 256);
						uart_count += 1;
						uart_count = uart_count % 8;
					}
					break;
				}
			}
		} // 接收到字符，MSComm控件发送事件 
	}
}

// 从串口发送数据
void CUartToUdpDlg::OnSendFromSerialPort(unsigned char* TXDATA, unsigned int uiCount)
{
	/*m_ctrlMSComm1.put_RTSEnable(FALSE);          //将串口通信端口设置成发送状态*/

	// 如果端口未打开则不发送数据
	if(m_ctrlMSComm1.get_PortOpen() == FALSE)
	{
		return;
	}

	CByteArray array;

	array.RemoveAll();

	array.SetSize(uiCount);

	for(unsigned int i=0; i<uiCount; i++)
	{
		array.SetAt(i, TXDATA[i]);
	}
	
	m_ctrlMSComm1.put_Output(COleVariant(array)); // 发送数据

}

// 从UDP发送数据
void CUartToUdpDlg::OnSendFromUDP(unsigned char* TXDATA, unsigned int uiCount)
{
	unsigned int uiFrameCount = 0;
	unsigned short uiPort;
	unsigned char ucTxdata[256];
	uiFrameCount = uiCount/256;
	if (uiFrameCount == 0)
	{
		return;
	}
	for (unsigned int i=0; i<uiFrameCount; i++)
	{
		memcpy(&uiPort, &TXDATA[24], 2);
		memcpy(&ucTxdata, &TXDATA[i*256], 256);
		for (int j=0; j<UDPPortNb; j++)
		{
			if (uiPort == m_uiPortNb[j])
			{
				m_pUDPSocket->SendTo(ucTxdata, 256, uiPort, _T("127.0.0.1"));
			}
		}
	}
	
}
