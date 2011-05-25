using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;




namespace UartToUdp_Net
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private uint m_icount = 0;
        private byte[] uart_buf = new byte[256];
        private byte[] udp_buf = new byte[128];
        private UdpClient udpReceive;
        private UdpClient udpSend;
        //创建一个Thread类
        private Thread thread1;
        // 在本机指定的端口发送
        private IPEndPoint remoteIPEndPointSend = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 0x9001);
        // 在本机指定的端口接收
        private IPEndPoint remoteIPEndPointRec = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 0X9002);

        private IPEndPoint iep = new IPEndPoint(IPAddress.Any, 0);

        private void button1_Click(object sender, EventArgs e)
        {
            // 先关闭已经打开的端口
            if (axMSComm1.PortOpen)
            {
                axMSComm1.PortOpen = false;
            }
            string sPort;
            string sBaud;
            short iPort = 0;
            sPort = comboBox1.Text;
            if (sPort == "端口1")
            {
                iPort = 1;
            }
            else if (sPort == "端口2")
            {
                iPort = 2;
            }
            else if (sPort == "端口3")
            {
                iPort = 3;
            }
            else if (sPort == "端口4")
            {
                iPort = 4;
            }
            else if (sPort == "端口5")
            {
                iPort = 5;
            }
            else if (sPort == "端口6")
            {
                iPort = 6;
            }
            else if (sPort == "端口7")
            {
                iPort = 7;
            }
            else if (sPort == "端口8")
            {
                iPort = 8;
            }
            else if (sPort == "端口9")
            {
                iPort = 9;
            }
            sBaud = comboBox2.Text;
            axMSComm1.CommPort = iPort;

            axMSComm1.Settings = sBaud + ",n,8,1";

            // 成功打开端口
            if (!axMSComm1.PortOpen)
            {
                // 打开串口
                axMSComm1.PortOpen = true;
                // 清空接收缓冲区
                axMSComm1.InBufferCount = 0;
                // 清空发送缓冲区
                axMSComm1.OutBufferCount = 0;
                MessageBox.Show("Open Serial Port Successful!");
                // 打开定时器1
                timer1.Enabled = true;
                timer1.Start();
                // 启动线程
                thread1.Start();
            }
        }
        private void OnSendFromUDP(byte[] TXDATA, int uiCount)
        {
            udpSend.Send(TXDATA, uiCount, remoteIPEndPointSend);    
        }
        private void OnTimer(object sender, EventArgs e)
        {
            object oResponse;
            short len;
            byte[] cRecBuf = new byte[256];
            len = axMSComm1.InBufferCount; //接收到的字符数目
            if (len > 0)
            {
                // 设置需要读取的输入缓冲区大小，为了不丢帧
                axMSComm1.InputLen = len;
                oResponse = axMSComm1.Input; //read

                cRecBuf = (byte[])oResponse;

                if (len > 1024)
                {
                    MessageBox.Show("Receive data from Uart leaked!");
                    return;
                }
                for (int i = 0; i < len; i++)
                {
                    switch (m_icount)
                    {
                        case 0:
                            // byte.Parse(str.Substring(i, 1)) == 0x11
                            if (cRecBuf[i] == 0x11)
                            {
                                uart_buf[0] = cRecBuf[i];
                                m_icount++;
                                break;
                            }
                            else
                            {
                                m_icount = 0;
                                break;
                            }

                        case 1:
                            if (cRecBuf[i] == 0x22)
                            {
                                uart_buf[1] = cRecBuf[i];
                                m_icount++;
                                break;
                            }
                            else
                            {
                                m_icount = 0;
                                break;
                            }
                        case 2:
                            if (cRecBuf[i] == 0x33)
                            {
                                uart_buf[2] = cRecBuf[i];
                                m_icount++;
                                break;
                            }
                            else
                            {
                                m_icount = 0;
                                break;
                            }
                        case 3:
                            if (cRecBuf[i] == 0x44)
                            {
                                uart_buf[3] = cRecBuf[i];
                                m_icount++;
                                break;
                            }
                            else
                            {
                                m_icount = 0;
                                break;
                            }
                        default:
                            uart_buf[m_icount] = cRecBuf[i];
                            m_icount++;
                            if (m_icount == 256)
                            {
                                m_icount = 0;
                                OnSendFromUDP(uart_buf, 256);
                            }
                            break;
                    }
                }
            }
        }
        private void ReceiveMessage()
        {

            while (true)
            {
                // ref表示引用类型的 IPPoint实例接收消息
                udp_buf = udpReceive.Receive(ref iep);
                axMSComm1.Output = udp_buf;
            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            udpSend = new UdpClient();
            udpReceive = new UdpClient(remoteIPEndPointRec);

            // 初始化该线程并指定线程执行时要调用的方法
            thread1 = new Thread(new ThreadStart(ReceiveMessage));
            
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            timer1.Enabled = false;
            udpSend.Close();
            udpReceive.Close();
            thread1.Abort();
        }
    }
}
