#include <vector>
using std::vector;
// ____________界面设置___________
// 设置纵向滚动条宽度
#define SetVScrollWidth							25
// 设置横向滚动条宽度
#define SetHScrollWidth						25
// 设置绘图控件X方向间隔
#define SetPlotCtrlXInterval					15
// 设置绘图控件Y方向间隔
#define SetPlotCtrlYInterval					15
// 设置绘图区X方向间隔
#define SetPlotAreaXInterval					10
// 设置绘图区Y方向间隔
#define SetPlotAreaYInterval					45
// 设置绘图区底部间隔
#define SetPlotAreaBottomInterval		30
// 设置标签宽度
#define SetLegendWidth						15	
// 设置标签X方向间隔
#define SetLegendXInterval					10
// 设置标签Y方向间隔
#define SetLegendYInterval					45
// 设置绘图控件背景颜色
#define SetPlotCtrlBackgroundColor		0xf0f0ff
// 设置绘图区背景颜色
#define SetPlotAreaBackgroundColor	0xffffff
// 设置横向栅格线颜色
#define SetHGridColor							0xcccccc
// 设置纵向栅格线颜色					
#define SetVGridColor							0xcccccc
// 设置标题栏背景颜色
#define SetTitleBackgroundColor			0xccccff
// 设置标题栏边框颜色
#define SetTitleEdgeColor						0x000000
// 设置标签栏字体大小
#define SetLegnedBoxFontSize				9
// 设置标题栏字体大小
#define SetTitleBoxFontSize					12
// 设置绘线的宽度
#define SetLineWidth								1
// 设置X轴的宽度
#define SetXAxisWidth							2
// 设置Y轴的宽度
#define SetYAxisWidth							2
// _________定义曲线颜色___________
// 红色RGB
#define RedColor								0xFF0000
// 绿色RGB
#define GreenColor							0x00FF00
// 蓝色RGB
#define BlueColor								0x0000FF
// 粉红RGB
#define PinkColor								0xFF8888
// _________ADC数据接受___________
// ADC数据图形化显示接收端口
#define ADCGraphShowPort				0x7800
// ADC数据图形化显示设置发送目标端口
#define ADCGraphSetRecPort			0x9401
// 本地IP地址
#define IPHostAddr							_T("127.0.0.1")
// 接收ADC数据一帧的缓冲区大小
#define ADCRecFrameBufSize			256
// 发送ADC数据图形显示设置帧的缓冲区大小
#define ADCSendFrameBufSize			256
// 接收数据包的ADC数据个数
#define ReceiveDataNum					72
// 每个文件存储每个仪器的ADC数据包个数
#define SaveADCDataPackageNum	406
// 每个数据在文件中所占空间大小
#define ADCDataSaveSize					10
// ADC数据端口接收缓冲区大小
#define ADCRecPortBufSize				10000 * ADCRecFrameBufSize
// ADC数据开始图形化显示标志位
#define StartGraphShow						3
// ADC数据停止图形化显示标志位
#define StopGraphShow						4
// 取最小值操作
#define OptMinValue							1
// 取最大值操作
#define OptMaxValue							2
// _________常量设置____________
// 设置读取文件头的缓冲区大小
#define SetReadFileHeadBufSize			1000
// 一个ADC数据占用缓冲区大小
#define ADCDataBufSize						12
// ADC数据间的间隔大小（空格+\t）
#define ADCDataInterval						2
// 网络传输显示帧数
#define ADCRecFrameShowNum			7
// 初始状态每条线显示的点数
#define ShowLinePointsNumNow			ADCRecFrameShowNum * ReceiveDataNum
// 每条线最少显示的点数
#define ShowLinePointsNumMin			10
// X轴坐标标签最小显示间隔数
#define SetLegendXIntervalNumMin		3
// 发送帧缓冲区初值设定
#define SndFrameBufInit						0x00
// 载入文件
#define OptTypeLoadFile						1
// 载入文件夹
#define OptTypeLoadFolder					2
// 网络传输绘图
#define OptTypeNetwork						3
//___________________帧格式设置___________________
// 帧头长度
#define FrameHeadSize				16
// 帧头校验字长度
#define FrameHeadCheckSize	4
// 帧头第一个字节
#define FrameHeadCheck0		0x11
// 帧头第二个字节
#define FrameHeadCheck1		0x22
// 帧头第三个字节
#define FrameHeadCheck2		0x33
// 帧头第四个字节
#define FrameHeadCheck3		0x44
// ADC数据图形化显示抽样率
#define CmdADCDataSamplingRate		0x20
// 命令字长度1字节
#define FrameCmdSize1B			1
// 命令包长度2字节
#define FramePacketSize2B		2
// 命令包长度4字节
#define FramePacketSize4B		4
// ADC数据所占字节数
#define ADCDataSize3B			3
//____________________命令字设置__________________
// 发送设置命令
#define SendSetCmd								0x0001
// 发送查询命令
#define SendQueryCmd							0x0002
// 发送ADC采样数据命令
#define SendADCCmd							0x0003
//___________________定时器设置____________________
// 界面刷新定时器序号
#define GraphRefreshTimerNb								1
// 界面刷新定时器延时设置
#define GraphRefreshTimerSet								500

// 设置Y轴下限
#define DrawGraphYAxisLower							-2.5
// 设置Y轴上限
#define DrawGraphYAxisUpper								2.5
// 采集站ADC数据存储
extern vector<double>* m_dbFduData;
// 采集站ADC数据绘图
extern vector<double>* m_dbFduShow;