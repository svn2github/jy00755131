// SerialPortSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "ATETEST.h"
#include "ATE.h"
#include "SerialPortSettings.h"


// CSerialPortSettings 对话框

IMPLEMENT_DYNAMIC(CSerialPortSettings, CDialog)

CSerialPortSettings::CSerialPortSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialPortSettings::IDD, pParent)
{

}

CSerialPortSettings::~CSerialPortSettings()
{
}

void CSerialPortSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERIALPORT_DEVICE, m_comboSerialPortDevice);
	DDX_Control(pDX, IDC_COMBO_SERIALPORT_COM, m_comboSerialPortCom);
	DDX_Control(pDX, IDC_COMBO_SERIALPORT_BAUD, m_comboSerialPortBaud);
	DDX_Control(pDX, IDC_COMBO_SERIALPORT_PARITY, m_comboSerialPortParity);
	DDX_Control(pDX, IDC_COMBO_SERIALPORT_BYTESIZE, m_comboSerialPortBytesize);
	DDX_Control(pDX, IDC_COMBO_SERIALPORT_STOPBITS, m_comboSerialPortStopbits);
}


BEGIN_MESSAGE_MAP(CSerialPortSettings, CDialog)
END_MESSAGE_MAP()


// CSerialPortSettings 消息处理程序
BOOL CSerialPortSettings::OnInitDialog()
{
	CDialog::OnInitDialog();

	//设备选项设置
	m_comboSerialPortDevice.SetWindowText( _T(Device0) );	//设置控件初始化显示
	m_comboSerialPortDevice.InsertString( 0, _T(Device0) );	//插入下拉菜单选项
	m_comboSerialPortDevice.InsertString( 1, _T(Device1) );
	
	//端口设置
	m_comboSerialPortCom.SetWindowText( _T(COM0) );		//设置控件初始化显示
	m_comboSerialPortCom.InsertString( 0, _T(COM0) );	//插入下拉菜单选项
	m_comboSerialPortCom.InsertString( 1, _T(COM1) );
	m_comboSerialPortCom.InsertString( 2, _T(COM2) );
	m_comboSerialPortCom.InsertString( 3, _T(COM3) );
	m_comboSerialPortCom.InsertString( 4, _T(COM4) );
	m_comboSerialPortCom.InsertString( 5, _T(COM5) );
	m_comboSerialPortCom.InsertString( 6, _T(COM6) );
	m_comboSerialPortCom.InsertString( 7, _T(COM7) );
	m_comboSerialPortCom.InsertString( 8, _T(COM8) );

	//波特率设置
	m_comboSerialPortBaud.SetWindowText( _T(BAUD0) );	//设置控件初始化显示
	m_comboSerialPortBaud.InsertString( 0, _T(BAUD0) );	//插入下拉菜单选项
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

	//校验位设置
	m_comboSerialPortParity.SetWindowText( _T(PARITY0) );	//设置控件初始化显示
	m_comboSerialPortParity.InsertString( 0, _T(PARITY0) );	//插入下拉菜单选项
	m_comboSerialPortParity.InsertString( 1, _T(PARITY1) );
	m_comboSerialPortParity.InsertString( 2, _T(PARITY2) );

	//数据位设置
	m_comboSerialPortBytesize.SetWindowText( _T(BYTESIZE0) );	//设置控件初始化显示
	m_comboSerialPortBytesize.InsertString( 0, _T(BYTESIZE0) );	//插入下拉菜单选项
	m_comboSerialPortBytesize.InsertString( 1, _T(BYTESIZE1) );
	m_comboSerialPortBytesize.InsertString( 2, _T(BYTESIZE2) );

	//停止位设置
	m_comboSerialPortStopbits.SetWindowText( _T(STOPBITS0) );	//设置控件初始化显示
	m_comboSerialPortStopbits.InsertString( 0, _T(STOPBITS0) );	//插入下拉菜单选项
	m_comboSerialPortStopbits.InsertString( 1, _T(STOPBITS1) );

	return TRUE;
}