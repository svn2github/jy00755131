
#ifndef Parameter_h
#define Parameter_h

// 是否管理员登陆标志位
extern bool m_bAdmin;
// 包头命令
#define FrameHead1					0x55
#define FrameHead2					0xAA
// 匹配命令
#define MatchCmd					0xAC
// 不匹配命令
#define NoMatchCmd					0xAB
// ProgramVPPStd命令字
#define ProgramVPPStdCmd			0xA1
// ProgramVPPLo命令字
#define ProgramVPPLoCmd				0xA2
// ReadVddLo命令字
#define ReadVddLoCmd				0xA3
// ReadVddHi命令字
#define ReadVddHiCmd				0xA4
// ReadMargin1命令字
#define ReadMargin1Cmd				0xA5
// ReadMargin2命令字
#define ReadMargin2Cmd				0xA6
// ReadMarginOffState命令字
#define ReadMarginOffStateCmd		0xA7
// 测试IRC命令字
#define TestIRCCmd					0xA8
// 烧写IRC命令字
#define ProgramIRCCmd				0xAA
// 烧写SCR命令字
#define ProgramSCRCmd				0xAD
// 结束命令字
#define EndCmd						0xAF
// Program选项
#define ProgramVPPStd		_T("OTP Program with VPP_std")
#define ProgramVPPLo		_T("OTP Program with VPP_Lo")
// Readback选项
#define ReadVddLo			_T("Read with Vdd_Lo")
#define ReadVddHi			_T("Read with Vdd_Hi")
#define ReadMargin1			_T("Margin-1 Read")
#define ReadMargin2			_T("Margin-2 Read")
#define	ReadMarginOffState	_T("off-state Margin Read")
// 自定义接收数据存储文件消息
#define WM_RECV_SAVE_DATA	WM_USER + 101
#endif
