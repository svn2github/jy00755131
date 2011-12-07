#pragma once

#include "MarkerList.h"
#include "AuxList.h"
#include "BlastMachineList.h"
#include "DetourList.h"
#include "MuteList.h"

/**
*@brief 布局设置类
*/
class CLayoutSetupData
{
public:
	CLayoutSetupData();
	~CLayoutSetupData();

public: // 属性
	/** XMLDOM文件对象指针*/
	CXMLDOMDocument* m_pXMLDOMDocument;

	/** 标记点列表对象*/
	CMarkerList m_oMarkerList;
	/** 辅助道列表对象*/
	CAuxList m_oAuxList;
	/** 爆炸机控制器列表对象*/
	CBlastMachineList m_oBlastMachineList;
	/** 迂回道列表对象*/
	CDetourList m_oDetourList;
	/** 哑道列表对象*/
	CMuteList m_oMuteList;

public: //方法
	// 初始化
	void OnInit();
	// 重新加载
	void OnReload();
	// 关闭
	void OnClose();

	// 解析配置文件
	void ParseXML();

	// 解析辅助道排列定义字符串，得到IP地址队列
	bool ParseSpreadToIPListByAuxiliaryDescr(CList<unsigned int, unsigned int>* plsIP, CString strAuxiliaryDescr);
	// 解析辅助道定义字符串，得到IP地址队列
	bool ParseSpreadToIPListByAuxiliaryDescrPart(CList<unsigned int, unsigned int>* plsIP, CString strPart);
};
