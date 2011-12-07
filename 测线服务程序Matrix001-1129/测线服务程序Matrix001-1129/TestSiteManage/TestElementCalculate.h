#pragma once

/**
*@brief 测试数据计算类
*/
class CTestElementCalculate
{
public:
	CTestElementCalculate();
	~CTestElementCalculate();

public: //属性
	/** 测试类型*/
	unsigned int m_uiType;

public: //方法
	// 初始化
	void OnInit();
	// 重置
	void Reset();
	// 关闭
	void OnClose();
	// 设置测试类型
	void SetTestType(unsigned int uiType);
	// 设置测试数据
	void SetTestData(byte* pData);
	// 得到测试结果
	float GetTestResult();
};
