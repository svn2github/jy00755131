#pragma once

/**
*@brief 测试请求类
*
@note 测试类型：
@note 0 正常记录。
@note 1 野外噪声。Noise
@note 2 野外脉冲。Pluse
@note 3 野外串音。Crosstalk
@note 4 仪器噪声。Noise
@note 5 仪器畸变。Distortion
@note 6 仪器增益/ 相位。Gain/Phase
@note 7 仪器串音。Crosstalk
@note 8 仪器共模。Common Mode Rejection
@note 9 合成。
@note 10 一致性（仅限 CM408）。
@note 11 仪器脉冲。Pluse
@note 12 野外畸变。Distortion
@note 13 仪器重力。
@note 14 野外漏电。Leakage
@note 15 野外电阻。Resistance

@note 16 仪器电阻。Resistance
@note 17 仪器漏电。Leakage
@note 18 仪器倾斜度。Tilt
@note 19 仪器倾斜度模式。Tilt Model
@note 20 野外倾斜度。Tilt
@note 21 野外倾斜度模式。Tilt Model
@note 22 野外噪声监测。

@note 仪器电阻
@note 仪器 CMRR 测试
@note 参考电压（仅限接通测线上电源或使用 TMS428 时）
@note 检波器倾斜度测试
@note 检波器 CMRR 测试（仅限 LT428 和 TMS428）

@note 30 仪器全部测试项目
@note 40 检波器全部测试项目
@note 45 新加检波器的初始测试
@note 50 仪器和检波器全部测试项目
@note 60 测试控制

@note 仪器测试设置中可选测试项
@note 4 仪器 Noise
@note 5 仪器 Distortion
@note 7 仪器 Crosstalk
@note 6 仪器 Gain/Phase 误差
@note 8 Common Mode Rejection

@note 检波器测试设置中可选测试项
@note 15 Resistance(ohms) 此测试允许您查看是否连接了检波器。采样率由用户选择。
@note 14 检波器 Leakage(Mohms) 此测试显示接收设备链路输入端与地面之间的大地漏电电阻。采样率由用户选择。
@note 1 检波器 Noise（微伏） 在此测试中，陆上检波器所检出的野外噪声是通过进行无点火命令的数据采集而测量的。采样率由用户选择。
@note 20 Tilt(%) 倾斜度测试
@note 21 Tilt Model 倾斜模型功能用于根据已知处在良好维修状态的几个陆上检波器@note 阵列，来存储对陆上检波器脉冲的响应的模型。
*/
class CTestRequest
{
public:
	CTestRequest();
	~CTestRequest();

public: //属性
	/** 索引号*/
	unsigned int m_uiIndex;
	/** 测试对象	1-仪器；2-检波器；3-全部*/
	unsigned int m_uiAim;
	/** 测试类型*/
	unsigned int m_uiType;
	/** 增益*/
	unsigned int m_uiGain;
	/** 跳转到的索引号*/
	unsigned int m_uiLoopNb;
	/** 循环次数*/
	unsigned int m_uiLoopCount;

	/** AD采集开始时刻*/
	unsigned int m_uiTimeADBegin;
	/** 采集开始时刻*/
	unsigned int m_uiTimeCollectBegin;
	/** 采集结束时刻*/
	unsigned int m_uiTimeCollectEnd;

public: //方法
	// 重置
	void OnReset();
	// 解析XML，仪器测试
	void ParseXMLForInstrument(CXMLDOMElement* pElement);
	// 解析XML，检波器测试
	void ParseXMLForSensor(CXMLDOMElement* pElement);
	// 解析XML，复合测试
	void ParseXMLForMultiple(CXMLDOMElement* pElement);
};
