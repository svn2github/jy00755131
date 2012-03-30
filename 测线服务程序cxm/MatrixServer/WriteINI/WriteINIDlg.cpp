
// WriteINIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteINI.h"
#include "WriteINIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWriteINIDlg 对话框




CWriteINIDlg::CWriteINIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWriteINIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWriteINIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWriteINIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_GENINI, &CWriteINIDlg::OnBnClickedBtnGenini)
END_MESSAGE_MAP()


// CWriteINIDlg 消息处理程序

BOOL CWriteINIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWriteINIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWriteINIDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWriteINIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWriteINIDlg::OnBnClickedBtnGenini()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSection = _T("");
	CString strSectionKey = _T("");
	CString strValue = _T("");
	CString strFilePath = _T("");
	wchar_t strBuff[256];

	GetCurrentDirectory(256,strBuff);		// 获取当前路径
	strFilePath.Format(_T("%s\\MatrixServerDLL.ini"),strBuff);

	strSection = _T("常量设置");			// 获取当前区域

	strSectionKey=_T("InstrumentCountAll");			// 仪器个数
	strValue = _T("20000");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCDataCountAll");			// ADC数据缓冲区个数
	strValue = _T("200000");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("OptTaskCountAll");			// 施工任务个数
	strValue = _T("20");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("OneSleepTime");				// 一次休眠的时间
	strValue = _T("50");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("LogOutPutSleepTimes");		// 日志输出线程写日志的延时次数
	strValue = _T("10");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("HertBeatSleepTimes");			// 心跳线程发送心跳帧延时次数
	strValue = _T("5");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("HeadFrameSleepTimes");		// 首包线程接收首包延时次数
	strValue = _T("10");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("IPSetFrameSleepTimes");		// IP地址设置线程延时次数
	strValue = _T("10");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("TailFrameSleepTimes");		// 尾包线程延时次数
	strValue = _T("10");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("MonitorSleepTimes");		// 路由监视线程延时次数
	strValue = _T("10");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("TimeDelaySleepTimes");		// 时统设置线程延时次数
	strValue = _T("4");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCSetSleepTimes");			// ADC参数设置线程延时次数
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ErrorCodeSleepTimes");		// 误码查询线程延时次数
	strValue = _T("20");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCDataRecSleepTimes");		// ADC数据接收线程延时次数
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCDataSaveSleepTimes");		// ADC数据存储线程延时次数
	strValue = _T("4");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CloseThreadSleepTimes");		// 等待线程关闭的延时次数
	strValue = _T("20");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("HeadFrameStableNum");		// 首包计数
	strValue = _T("4");
	//写入ini文件中相应字段
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("IPAddrResetTimes");		// IP地址重设次数
	strValue = _T("3");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("TailFrameStableTimes");	// 尾包稳定次数
	strValue = _T("5");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("MonitorStableTime");	// 路由监视稳定时间
	strValue = _T("5000");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("LineSysStableTime");		// 测网系统达到稳定状态时间
	strValue = _T("5000");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCSetOptNb");			// ADC参数设置操作序号
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCStartSampleOptNb");	// ADC开始采集操作序号
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCStopSampleOptNb");		// ADC停止采集操作序号
	strValue = _T("3");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("InstrumentTypeLAUX");		// 仪器类型-交叉站
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("InstrumentTypeLAUL");		// 仪器类型-电源站
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("InstrumentTypeFDU");		// 仪器类型-采集站
	strValue = _T("3");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("InstrumentTypeLCI");		// 仪器类型-LCI
	strValue = _T("4");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("DirectionTop");			// 方向上方
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("DirectionDown");			// 方向下方
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("DirectionLeft");			// 方向左方
	strValue = _T("3");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("DirectionRight");			// 方向右方
	strValue = _T("4");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("DirectionCenter");		// 方向正中
	strValue = _T("0");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("IPSetAddrStart");			// IP地址设置的起始地址
	strValue = _T("71");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("IPSetAddrInterval");		// IP地址设置的间隔
	strValue = _T("16");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("RoutSetAddrStart");		// 路由地址设置的起始地址
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("RoutSetAddrInterval");	// 路由地址设置的间隔
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("BroadcastPortStart");		// 设置广播端口起始地址
	strValue = _T("16");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("IPBroadcastAddr");		// 设置为广播IP
	strValue = _T("4294967295");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCFrameSaveInOneFileNum");	// 一个文件内存储单个设备ADC数据帧数
	strValue = _T("200");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCSaveHeadLineNum");		// 存储ADC数据的文件头行数
	strValue = _T("3");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCSaveLeftInfoBytes");	// 存储ADC数据的左侧预留信息字节数
	strValue = _T("14");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCSaveDataBytes");		// 存储ADC数据的字节数
	strValue = _T("10");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCSaveDataIntervalBytes");	// 存储ADC数据之间的间隔字节数
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCDataBufSize");			// 设备ADC数据缓冲区大小
	strValue = _T("2048");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("TBSleepTimeHigh");			// TB设置延时高位
	strValue = _T("16384");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("TBSleepTimeLow");				// TB设置延时低位
	strValue = _T("250");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSection = _T("帧格式设置");			// 获取当前区域

	strSectionKey=_T("FrameHeadSize");		// 帧头长度
	strValue = _T("16");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("FrameHeadCheck");		// 同步帧头
	strValue = _T("0x11,0x22,0x33,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00");    
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);
	
	strSectionKey=_T("FrameCmdSize1B");		// 命令字长度1字节
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("FramePacketSize1B");	// 命令包长度1字节
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("FramePacketSize2B");	// 命令包长度2字节
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("FramePacketSize4B");	// 命令包长度4字节
	strValue = _T("4");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCDataSize3B");		// ADC数据所占字节数
	strValue = _T("3");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCDataInOneFrameNum");	// 一帧内ADC数据个数
	strValue = _T("72");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("ADCFramePointLimit");		// ADC数据帧指针偏移量上限
	strValue = _T("2048");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CommandWordMaxNum");	// 命令字个数最大值
	strValue = _T("41");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("SndFrameBufInit");	// 发送帧缓冲区初值设定
	strValue = _T("0");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("RcvFrameSize");		// 接收的网络数据帧帧长度
	strValue = _T("256");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("SndFrameSize");		// 发送的网络数据帧帧长度
	strValue = _T("128");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	/////////////////////////////////////////////////////////////////////////
	strSection = _T("服务器与设备命令字设置");		// 获取当前区域
	strSectionKey=_T("SendSetCmd");			// 发送设置命令
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("SendQueryCmd");		// 发送查询命令
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("SendADCCmd");			// 发送ADC采样数据重发命令
	strValue = _T("3");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdSn");				// 串号
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdHeadFrameTime");	// 首包时间
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLocalIPAddr");		// 本地IP地址
	strValue = _T("3");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLocalSysTime");	// 本地系统时间
	strValue = _T("4");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLocalTimeFixedHigh");	// 本地时间修正高位
	strValue = _T("5");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLocalTimeFixedLow");	// 本地时间修正低位
	strValue = _T("6");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdADCDataReturnAddr");	// 自动数据返回地址
	strValue = _T("7");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdADCDataReturnPort");	// 自动数据返回端口和命令
	strValue = _T("8");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdADCDataReturnPortLimit");	// 端口递增下限和上限
	strValue = _T("9");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdSetBroadCastPort");	// 设置网络等待端口和命令
	strValue = _T("10");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdFDUErrorCode");		// 系统硬件状态拷贝
	strValue = _T("11");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdTBHigh");				// TB时刻高位
	strValue = _T("12");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdTbLow");				// TB时刻低位
	strValue = _T("13");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLAUXRoutOpenQuery");	// work_ctrl 交叉站方向
	strValue = _T("14");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLAUXRoutOpenSet");		// 路由开关
	strValue = _T("15");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdTailRecSndTimeLow");	// 尾包接收\发送时刻低位
	strValue = _T("22");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdBroadCastPortSet");	// 广播命令等待端口匹配
	strValue = _T("23");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdADCSet");				// 设置ADC控制命令命令字
	strValue = _T("24");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdNetTime");				// 网络时刻
	strValue = _T("25");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLineTailRecTimeLAUX");	// 交叉站大线尾包接收时刻
	strValue = _T("27");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLAUTailRecTimeLAUX");	// 交叉站交叉线尾包接收时刻
	strValue = _T("28");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLAUXErrorCode1");		// 交叉站故障1
	strValue = _T("29");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLAUXErrorCode2");		// 交叉站故障2
	strValue = _T("30");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdLAUXSetRout");			// 交叉站路由分配
	strValue = _T("31");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdReturnRout");			// 返回路由
	strValue = _T("63");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdEnd");					// 命令解析结束命令
	strValue = _T("0");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdTBCtrlStartSample");		// TB开关控制ADC数据采集命令
	strValue = _T("1");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);
	
	strSectionKey=_T("CmdTBLoseCtrlStartSample");	// 无需TB开关控制ADC数据采集命令
	strValue = _T("2");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdTBCtrlStopSample");		// TB开关控制ADC数据停止采集命令
	strValue = _T("0");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

	strSectionKey=_T("CmdCtrlCloseLed");			// LED灯灭
	strValue = _T("4");
	WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);
}
